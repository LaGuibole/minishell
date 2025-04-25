/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameters.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlintot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 12:43:05 by mlintot           #+#    #+#             */
/*   Updated: 2025/04/25 12:43:06 by mlintot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_env_parameters(t_cmd *cmd)
{
	(void) cmd;
	return (RET_OK);
}

char	*clean_parameters(char *str)
{
	int		i;
	bool	single_quote;
	bool	double_quote;
	char	*cleaned;
	int		j;

	i = 0;
	single_quote = 0;
	double_quote = 0;
	j = 0;
	cleaned = malloc(sizeof(char) * (ft_strlen(str) + 1));
	while (*str)
	{
		if (*str == '\'' && !double_quote)
			single_quote = !single_quote;
		else if (*str == '"' && !single_quote)
			double_quote = !double_quote;
		if (!single_quote && !double_quote && is_redir_char(*str))
		{
			if (*str == *str + 1)
				str += 2;
			else
				str++;
			while (*str == ' ')
				str++;
			while (*str && *str != ' ' && !is_redir_char(*str))
				str++;
			continue ;
		}
		else
			cleaned[j++] = *str++;
	}
	cleaned[j] = '\0';
	return (cleaned);
}

int	set_parameters(char *str, t_cmd *cmd)
{
	size_t	start;
	size_t	end;
	char	**params;
	bool	single_quote;
	bool	double_quote;

	str = clean_parameters(str);
	params = malloc(sizeof(char *) * ft_strlen(str) + 1);
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
		end = 0;
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
		if (ft_strlen(params[cmd->nbparams - 1]) == 0)
			free(params[cmd->nbparams--]);
		else if (!params[cmd->nbparams - 1])
		{
			cmd->params = params;
			cmd->nbparams--;
			fd_printf(STDOUT_FILENO, ERR_MALLOC);
			free(str);
			return (RET_ERR);
		}
		start += ++end;
	}
	params[cmd->nbparams] = NULL;
	cmd->params = params;
	free(str);
	return (RET_OK);
}
