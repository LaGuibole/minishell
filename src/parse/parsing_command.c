/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 22:15:45 by mlintot           #+#    #+#             */
/*   Updated: 2025/04/23 11:54:08 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_is_builtin(t_cmd *cmd)
{
	cmd->is_builtin = cmd_is_builtin(cmd->cmd);
	return (RET_OK);
}

static int	set_cmd(char *str, t_cmd *cmd)
{
	int	start;
	int end;

	start = 0;
	while (str[start] && str[start] == ' ')
		start++;
	end = 0;
	while (str[start + end] && str[start + end] != ' ')
		end++;
	cmd->cmd = ft_substr(str, start, end);
	if (cmd->cmd)
		return (RET_OK);
	return (RET_ERR);
}

static int	set_env_parameters(t_cmd *cmd)
{
	(void) cmd;
	return (RET_OK);
}

static bool	is_redir_char(char c)
{
	return (c == '<' || c == '>');
}

static char	*clean_parameters(char *str)
{
	int		i = 0;
	bool	single_quote = 0;
	bool	double_quote = 0;
	char	*cleaned;
	int		j = 0;

	cleaned = malloc(sizeof(char) * (ft_strlen(str) + 1));
	while (str[i])
	{
		if (str[i] == '\'' && !double_quote)
			single_quote = !single_quote;
		else if (str[i] == '"' && !single_quote)
			double_quote = !double_quote;

		if (!single_quote && !double_quote && is_redir_char(str[i]))
		{
			if (str[i] == str[i + 1])
				i += 2;
			else
				i += 1;
			while (str[i] == ' ')
				i++;
			while (str[i] && str[i] != ' ' && !is_redir_char(str[i]))
				i++;

			continue;
		}
		else
			cleaned[j++] = str[i++];
	}
	cleaned[j] = '\0';
	return (cleaned);
}

static int	set_parameters(char *str, t_cmd *cmd)
{
	(void) str;
	(void) cmd;

	size_t		start;
	char	**params;
	bool	single_quote;
	bool	double_quote;

	str = clean_parameters(str);
	params = malloc(sizeof(char *) + 2);
	if (!params)
	{
		fd_printf(STDOUT_FILENO, ERR_MALLOC);
		return (RET_ERR);
	}
	start = 0;
	while (str[start] && str[start] == ' ')
		start++;
	single_quote = 0;
	double_quote = 0;
	while (start < ft_strlen(str))
	{
		size_t end = 0;

		while ((str[start + end] && str[start + end] != ' ') || \
				double_quote || single_quote)
		{
			if (str[start + end] == '\'' && !double_quote)
				single_quote = !single_quote;
			if (str[start + end] == '"' && !single_quote)
				double_quote = !double_quote;
			end++;
		}
		params[cmd->nbparams++] = ft_substr(str, start, end);
		if (!params[cmd->nbparams - 1])
		{
			fd_printf(STDOUT_FILENO, ERR_MALLOC);
			free(str);
			free(params); //a refaire propre
			return (RET_ERR);
		}
		start += ++end;
	}
	params[cmd->nbparams] = NULL;
	cmd->params = params;
	free(str);
	return (RET_OK);
}

static int	add_redir(t_cmd *cmd, t_redir_type type, char *filename)
{
	t_redir *new = malloc(sizeof(t_redir));
	if (!new)
		return (RET_ERR);
	new->type = type;
	new->filename = filename;
	new->next = NULL;

	if (!cmd->redir)
		cmd->redir = new;
	else
	{
		t_redir *tmp = cmd->redir;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (RET_OK);
}

static int	set_redirect(char *str, t_cmd *cmd)
{
	int		i = 0;
	bool	single_quote = 0;
	bool	double_quote = 0;

	while (str[i])
	{
		if (str[i] == '\'' && !double_quote)
			single_quote = !single_quote;
		else if (str[i] == '"' && !single_quote)
			double_quote = !double_quote;
		if (!single_quote && !double_quote && (str[i] == '<' || str[i] == '>'))
		{
			t_redir_type type;
			if (str[i] == '<' && str[i + 1] == '<')
			{
				type = R_HEREDOC;
				i += 2;
			}
			else if (str[i] == '>' && str[i + 1] == '>')
			{
				type = R_APPEND;
				i += 2;
			}
			else if (str[i] == '<')
			{
				type = R_INPUT;
				i++;
			}
			else
			{
				type = R_OUTPUT;
				i++;
			}
			while (str[i] == ' ')
				i++;
			if (str[i] == '<' || str[i] == '>' || str[i] == '\0')
			{
				if (str[i] == '\0')
					fd_printf(STDOUT_FILENO, "%s `newline'\n", ERR_CHEV);
				else if ((str[i] == '<' && str[i + 1] == '<') || (str[i] == '>' && str[i + 1] == '>'))
					fd_printf(STDOUT_FILENO, "%s `%c%c'\n", ERR_CHEV, str[i], str[i]);
				else
					fd_printf(STDOUT_FILENO, "%s `%c'\n", ERR_CHEV, str[i]);
				return (RET_ERR);
			}
			int fname_start = i;
			while (str[i] && str[i] != ' ' && str[i] != '<' && str[i] != '>')
				i++;
			char *filename = ft_substr(str, fname_start, i - fname_start);
			if (!filename)
				return (RET_ERR);
			if (add_redir(cmd, type, filename) != RET_OK)
			{
				free(filename);
				return (RET_ERR);
			}
		}
		else
			i++;
	}
	return (RET_OK);
}

static int	split_cmd(char **line, char *str)
{
	char	*tmp;
	bool	single_quote;
	bool	double_quote;
	int		start;
	int 	cpt;
	int		nbline;

	tmp = str;
	start = 0;
	cpt = 0;
	nbline = 0;
	single_quote = 0;
	double_quote = 0;
	while (*tmp)
	{
		if (*tmp == '\'' && !double_quote)
			single_quote = !single_quote;
		if (*tmp == '"' && !single_quote)
			double_quote = !double_quote;
		cpt++;
		if (*tmp == '|' && !double_quote && !single_quote)
		{
			line[nbline++] = ft_substr(str, start, cpt - 1);
			start = start + cpt;
			cpt = 0;
		}
		tmp++;
	}
	line[nbline++] = ft_substr(str, start, cpt);
	return (nbline);
}

int	parse_cmd(char *str, t_cmd **cmd)
{
	char	**line;
	int		nbline;
	int		cpt;
	t_cmd	*current;

	line = malloc(sizeof(char *) + 1);
	if (!line)
	{
		fd_printf(STDOUT_FILENO, ERR_MALLOC);
		return (RET_ERR);
	}
	nbline = split_cmd(line, str);
	cpt = 0;
	while (cpt < nbline)
	{
		current = cmdnew();
		if (!current)
		{
			fd_printf(STDOUT_FILENO, ERR_MALLOC);
			return (RET_ERR);
		}
		current->nbparams = 0;
		if (
			set_cmd(line[cpt], current) || //Recuperation command
			set_redirect(line[cpt], current) || //Recuperation des redirections de la command
			set_parameters(line[cpt], current) || //Recuperation des parametres de la command
			set_is_builtin(current) || //Gestion is_builtin
			set_env_parameters(current) // Gestion variable env
		)
		{
			fd_printf(STDOUT_FILENO, ERR_PARSE); //A supprimer a la fin des tests
			free(line[cpt]); // A faire propre
			free(current); // A faire propre
			return (RET_ERR);
		}
		if (!cmd)
			cmd = &current;
		else
			cmdadd_back(cmd, current);
		cpt++;
	}
	return (RET_OK);
}
