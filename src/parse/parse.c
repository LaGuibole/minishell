/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlintot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:47:16 by mlintot           #+#    #+#             */
/*   Updated: 2025/04/14 14:47:17 by mlintot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	empty_link(char *str)
{
	bool	pipe;
	bool	single_quote;
	bool	double_quote;

	pipe = 0;
	while (*str == ' ')
		str++;
	if (*str == '|')
	{
		fd_printf(STDOUT_FILENO, ERR_PIPE);
		return (RET_ERR);
	}
	single_quote = 0;
	double_quote = 0;
	while (*str)
	{
		if (*str == '\'' && !double_quote)
			single_quote = !single_quote;
		if (*str == '"' && !single_quote)
			double_quote = !double_quote;
		if (*str == '&' && !single_quote && !double_quote)
			return (RET_ERR);
		if (*str == '|' && !single_quote && !double_quote)
		{
			if (pipe == 1)
			{
				fd_printf(STDOUT_FILENO, ERR_PIPE);
				return (pipe);
			}
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
	bool	single_quote;
	bool	double_quote;

	single_quote = 0;
	double_quote = 0;
	while (*str)
	{
		if (*str == '\'' && !double_quote)
			single_quote = !single_quote;
		if (*str == '"' && !single_quote)
			double_quote = !double_quote;
		str++;
	}
	if (single_quote || double_quote)
		fd_printf(STDOUT_FILENO, ERR_QUOTE);
	return (single_quote || double_quote);
}

t_cmd	*parsing_cmd(char *str)
{
	t_cmd	*cmd;

	cmd = NULL;
	if (
		unclosed_quotes(str) || //Gestion ' et " pour readline
		empty_link(str) || //Gestion des pipe et esperluette
		parse_cmd(str, &cmd) //Separation cmd, parameters et redirection dans cmd
	)
	{
		if (cmd)
			free(cmd);
		return (NULL);
	}
	return (cmd);
}
