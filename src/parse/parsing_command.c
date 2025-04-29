/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 22:15:45 by mlintot           #+#    #+#             */
/*   Updated: 2025/04/28 15:13:43 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_is_builtin(t_cmd *cmd)
{
	cmd->is_builtin = cmd_is_builtin(cmd->cmd);
	return (RET_OK);
}

static int	set_cmd(t_cmd *cmd)
{
	if (cmd->params[0])
		cmd->cmd = ft_strdup(cmd->params[0]);
	return (RET_OK);
}

static int	set_redirect(char *str, t_cmd *cmd)
{
	int				i;
	bool			single_quote;
	bool			double_quote;
	t_redir_type	type;
	int				fname_start;
	char			*filename;

	i = 0;
	single_quote = 0;
	double_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !double_quote)
			single_quote = !single_quote;
		else if (str[i] == '"' && !single_quote)
			double_quote = !double_quote;
		if (!single_quote && !double_quote && (str[i] == '<' || str[i] == '>'))
		{
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
				else if ((str[i] == '<' && str[i + 1] == '<') || \
(str[i] == '>' && str[i + 1] == '>'))
					fd_printf(STDOUT_FILENO, "%s `%c%c'\n", \
ERR_CHEV, str[i], str[i]);
				else
					fd_printf(STDOUT_FILENO, "%s `%c'\n", ERR_CHEV, str[i]);
				return (RET_ERR);
			}
			fname_start = i;
			while (str[i] && str[i] != ' ' && str[i] != '<' && str[i] != '>')
				i++;
			filename = ft_substr(str, fname_start, i - fname_start);
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
	int		cpt;
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

	line = malloc(sizeof(char *) * (ft_strlen(str) + 1));
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
			free_strstr(line, nbline);
			return (RET_ERR);
		}
		current->nbparams = 0;
		if (
			set_redirect(line[cpt], current) || \
set_parameters(line[cpt], current) || \
set_cmd(current) || \
set_is_builtin(current) || \
set_env_parameters(current)
		)
		{
			fd_printf(STDOUT_FILENO, ERR_PARSE);
			free_strstr(line, nbline);
			return (RET_ERR);
		}
		if (!cmd)
			cmd = &current;
		else
			cmdadd_back(cmd, current);
		cpt++;
	}
	free_strstr(line, nbline);
	return (RET_OK);
}
