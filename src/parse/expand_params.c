/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_params.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:43:04 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/07 16:35:57 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	purge_quote(char *p, int k, t_cmd *cmd)
{
	size_t	i;
	size_t	j;
	char	*newline;

	i = 0;
	newline = ft_calloc(sizeof(char), 1);
	while (i < ft_strlen(p))
	{
		if (ft_quote(p[i], 1))
		{
			j = ++i;
			while (ft_quote('\'', 0) || ft_quote('"', 0))
			{
				ft_quote(p[j], 1);
				if (ft_quote('\'', 0) || ft_quote('"', 0))
					j++;
				else
					newline = free_join(newline, ft_substr(p, i, j - i), 1, 1);
			}
			i += j;
		}
		else
			newline = free_join(newline, ft_substr(p, i++, 1), 1, 1);
	}
	return (free(p), cmd->params[k] = newline, RET_OK);
}

static char	*get_var_value(char *key)
{
	char	*ret;

	if (!key)
		return (NULL);
	if (ft_strcmp(key, "?") == 0)
		return (free(key), ft_itoa(g_signal));
	ret = ft_getenv(key);
	return (free(key), ret);
}

static int	check_env_params(char c)
{
	return (c && c != ' ' && c != '\'' && c != '$' && c != '"');
}

static int	set_env(char *params, int k, t_cmd *cmd)
{
	size_t	i;
	int		tmp;
	char	*env;
	char	*newline;

	newline = ft_calloc(sizeof(char), 1);
	i = 0;
	while (i < ft_strlen(params))
	{
		ft_quote(params[i], 1);
		if (params[i] == '$' && !ft_quote('\'', 0))
		{
			tmp = ++i;
			while (check_env_params(params[i]))
				i++;
			env = get_var_value(ft_substr(params, tmp, i - tmp));
			if (!env)
				newline = free_join(newline, "", 1, 0);
			else
				newline = free_join(newline, env, 1, 1);
		}
		else
			newline = free_join(newline, ft_substr(params, i++, 1), 1, 1);
	}
	return (free(params), cmd->params[k] = newline, RET_OK);
}

int	set_expand(t_cmd *cmd)
{
	int		i;

	i = 0;
	while (i < cmd->nbparams)
	{
		if (set_env(cmd->params[i], i, cmd)
			|| purge_quote(cmd->params[i], i, cmd))
			return (RET_ERR);
		i++;
	}
	return (RET_OK);
}
