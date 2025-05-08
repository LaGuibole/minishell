/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:47:16 by mlintot           #+#    #+#             */
/*   Updated: 2025/05/08 19:22:27 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_quote(char type, bool upd)
{
	static bool	single_quote = 0;
	static bool	double_quote = 0;

	if (type == '\'' && !double_quote)
	{
		if (upd)
			single_quote = !single_quote;
		return (single_quote);
	}
	if (type == '"' && !single_quote)
	{
		if (upd)
			double_quote = !double_quote;
		return (double_quote);
	}
	return (0);
}

bool	print_error(char *str)
{
	fd_printf(STDOUT_FILENO, str);
	return (RET_ERR);
}

static bool	empty_link(char *str)
{
	bool	pipe;

	pipe = 0;
	while (*str == ' ')
		str++;
	if (*str == '|')
		return (print_error(ERR_PIPE));
	while (*str)
	{
		ft_quote(*str, 1);
		if (*str == '&' && !ft_quote('\'', 0) && !ft_quote('"', 0))
			return (print_error(ERR_PARSE));
		if (*str == '|' && !ft_quote('\'', 0) && !ft_quote('"', 0))
		{
			if (pipe == 1)
				return (print_error(ERR_PIPE));
			pipe = 1;
		}
		if (*str != '|' && *str != ' ')
			pipe = 0;
		str++;
	}
	return (pipe);
}

static bool	unclosed_quotes(char *str)
{
	while (*str)
		ft_quote(*str++, 1);
	if (ft_quote('\'', 0) || ft_quote('"', 0))
		return (print_error(ERR_QUOTE));
	return (ft_quote('\'', 0) || ft_quote('"', 0));
}

t_cmd	*parsing_cmd(char *str)
{
	t_cmd	*cmd;

	cmd = NULL;
	if (unclosed_quotes(str) || empty_link(str) || parse_cmd(str, &cmd))
	{
		if (ft_quote('\'', 0))
			ft_quote('\'', 1);
		if (ft_quote('"', 0))
			ft_quote('"', 1);
		if (cmd)
			free_cmd_list(cmd);
		g_signal = 2;
		return (NULL);
	}
	return (cmd);
}
