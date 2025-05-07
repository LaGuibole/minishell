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

static char	*ft_strjoin_char(char *s, char c);

char	*get_var_value(char *key)
{
	if (!key)
		return (NULL);
	if (ft_strcmp(key, "?") == 0)
		return (ft_itoa(g_signal));
	return (ft_getenv(key));
}

int	is_valid_var_char(char c, int pos)
{
	if (pos == 0)
		return (ft_isalpha(c) || c == '_' || c == '?');
	return (ft_isalnum(c) || c == '_');
}

char	*expand_loop(char *str, char *res)
{
	int		i;
	int		j;
	char	*key;
	char	*val;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' && ft_quote('"', 0))
			ft_quote('\'', 1);
		else if (str[i] == '"' && !ft_quote('\'', 0))
			ft_quote('"', 1);
		else if (str[i] == '$' && !ft_quote('\'', 0))
		{
			j = 1;
			while (str[i + j] && is_valid_var_char(str[i + j], j - 1))
				j++;
			key = ft_substr(str, i + 1, j - 1);
			val = get_var_value(key);
			if (val)
			{
				my_strcat(&res, val);
				free(val);
			}
			free(key);
			i += j;
			continue ;
		}
		else
			res = ft_strjoin_char(res, str[i]);
		i++;
	}
	return (res);
}

char	*expand_param(char *param)
{
	char	*res;

	ft_quote(0, 1);
	res = ft_strdup("");
	if (!res)
		return (NULL);
	res = expand_loop(param, res);
	return (res);
}

static	char	*ft_strjoin_char(char *s, char c)
{
	char	*res;
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	res = malloc(sizeof(char) * (len + 2));
	if (!res)
		return (NULL);
	ft_strlcpy(res, s, len + 1);
	res[len] = c;
	res[len + 1] = '\0';
	free(s);
	return (res);
}
