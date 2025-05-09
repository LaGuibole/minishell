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
			i = ++j;
		}
		else
			newline = free_join(newline, ft_substr(p, i++, 1), 1, 1);
	}
	return (free(p), cmd->params[k] = newline, RET_OK);
}

char	*get_var_value(char *key)
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

static int	set_env(char *p, int k, t_cmd *cmd)
{
	size_t	i;
	size_t	tmp;
	char	*l;

	l = ft_calloc(sizeof(char), 1);
	i = 0;
	while (i < ft_strlen(p))
	{
		ft_quote(p[i], 1);
		if (p[i] == '$' && !ft_quote('\'', 0))
		{
			tmp = ++i;
			while (check_env_params(p[i]))
				i++;
			if (i == tmp)
			{
				l = free_join(l, "$", 1, 0);
				continue ;
			}
			l = get_env(ft_substr(p, tmp, i - tmp), l);
		}
		else
			l = free_join(l, ft_substr(p, i++, 1), 1, 1);
	}
	return (free(p), cmd->params[k] = l, RET_OK);
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
