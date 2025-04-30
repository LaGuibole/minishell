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

static int	check_more_chev(char c, char next)
{
	if (c == '<' || c == '>' || c == '\0')
	{
		if (c == '\0')
			fd_printf(STDOUT_FILENO, "%s `newline'\n", ERR_CHEV);
		else if ((c == '<' && next == '<') || \
(c == '>' && next == '>'))
			fd_printf(STDOUT_FILENO, "%s `%c%c'\n", \
ERR_CHEV, c, c);
		else
			fd_printf(STDOUT_FILENO, "%s `%c'\n", ERR_CHEV, c);
		return (RET_ERR);
	}
	return (RET_OK);
}

static int	set_type_chev(t_redir_type *type, char c, char next)
{
	int	i;

	i = 1;
	if (c == '<' && next == '<' && i++)
		*type = R_HEREDOC;
	else if (c == '>' && next == '>' && i++)
		*type = R_APPEND;
	else if (c == '<')
		*type = R_INPUT;
	else
		*type = R_OUTPUT;
	return (i);
}

static int	set_redirect(char *str, t_cmd *cmd)
{
	size_t				i;
	t_redir_type	type;
	int				fname_start;
	char			*filename;

	i = 0;
	while (str[i])
	{
		ft_quote(str[i], 1);
		if (!ft_quote('\'', 0) && !ft_quote('"', 0) && \
(str[i] == '<' || str[i] == '>'))
		{
			i += set_type_chev(&type, str[i], str[i + 1]);
			while (str[i] == ' ')
				i++;
			if (ft_strlen(str) <= i)
			{
				if (check_more_chev(str[i], '\0'))
					return (RET_ERR);
				else if (check_more_chev(str[i], str[i + 1]))
					return (RET_ERR);
			}
			fname_start = i;
			while (str[i] && str[i] != ' ' && str[i] != '<' && str[i] != '>')
				i++;
			filename = ft_substr(str, fname_start, i - fname_start);
			if (!filename)
				return (RET_ERR);
			if (add_redir(cmd, type, filename))
				return (free(filename), RET_ERR);
		}
		else
			i++;
	}
	return (RET_OK);
}

static int	split_cmd(char **line, char *str)
{
	char	*tmp;
	int		start;
	int		cpt;
	int		nbline;

	tmp = str;
	start = 0;
	cpt = 0;
	nbline = 0;
	while (*tmp)
	{
		ft_quote(*tmp, 1);
		cpt++;
		if (*tmp == '|' && !ft_quote('\'', 0) && !ft_quote('"', 0))
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
		return (print_error(ERR_MALLOC));
	nbline = split_cmd(line, str);
	cpt = 0;
	while (cpt < nbline)
	{
		current = cmdnew();
		if (!current)
			return (free_strstr(line, nbline), print_error(ERR_MALLOC));
		current->nbparams = 0;
		if (set_redirect(line[cpt], current) || \
set_parameters(line[cpt], current) || set_cmd(current) || \
set_is_builtin(current) || set_env_parameters(current))
			return (free_strstr(line, nbline), print_error(ERR_PARSE));
		cmdadd_back(cmd, current);
		cpt++;
	}
	free_strstr(line, nbline);
	return (RET_OK);
}
