/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameters.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 12:43:05 by mlintot           #+#    #+#             */
/*   Updated: 2025/04/28 15:21:42 by guphilip         ###   ########.fr       */
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
	char	*cleaned;
	int		j;

	j = 0;
	cleaned = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!cleaned)
		return (NULL);
	while (*str)
	{
		ft_quote(*str, 1);
		if (!ft_quote('\'', 0) && !ft_quote('"', 0) && is_redir_char(*str))
		{
			str += (*str == *str + 1) + 1;
			while (*str && *str == ' ')
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

	str = clean_parameters(str);
	if (!str)
		return (print_error(ERR_MALLOC));
	params = malloc(sizeof(char *) * (ft_strlen(str) + 1));
	if (!params)
		return (print_error(ERR_MALLOC));
	start = 0;
	while (str[start] && str[start] == ' ')
		start++;
	while (start < ft_strlen(str))
	{
		end = 0;
		while ((str[start + end] && str[start + end] != ' ') || \
ft_quote('\'', 0) || ft_quote('"', 0))
		{
			ft_quote(str[start + end], 1);
			end++;
		}
		params[cmd->nbparams++] = ft_substr(str, start, end);
		if (!params[cmd->nbparams - 1])
		{
			cmd->params = params;
			cmd->nbparams--;
			free(str);
			return (print_error(ERR_MALLOC));
		}
		else if (ft_strlen(params[cmd->nbparams - 1]) == 0)
			free(params[cmd->nbparams--]);
		start += ++end;
	}
	params[cmd->nbparams] = NULL;
	cmd->params = params;
	free(str);
	return (RET_OK);
}
