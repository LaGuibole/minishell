/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameters.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 12:43:05 by mlintot           #+#    #+#             */
/*   Updated: 2025/05/07 16:33:54 by guphilip         ###   ########.fr       */
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

int	construct_params(char **params, char *str, t_cmd *cmd, size_t start)
{
	while (start < ft_strlen(str))
	{
		while (str[start] == ' ')
			start++;
		if (!str[start])
			break ;
		if (set_expanded_param(params, str, cmd, &start) == -1)
		{
			cmd->params = params;
			cmd->nbparams--;
			free(str);
			return (print_error(ERR_MALLOC));
		}
	}
	return (RET_OK);
}

int	set_parameters(char *str, t_cmd *cmd)
{
	size_t	start;
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
	if (construct_params(params, str, cmd, start))
		return (RET_ERR);
	params[cmd->nbparams] = NULL;
	cmd->params = params;
	free(str);
	return (RET_OK);
}

int	set_expanded_param(char **params, char *str, t_cmd *cmd, size_t *start)
{
	char	*token;

	token = get_full_token(str, start);
	if (!token)
		return (-1);
	if (ft_strlen(token) == 0)
		free(token);
	else
		params[cmd->nbparams++] = token;
	return (0);
}
