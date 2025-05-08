/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameters.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 12:43:05 by mlintot           #+#    #+#             */
/*   Updated: 2025/05/08 20:59:32 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Concatenates 'src' to '*dst_ptr', frees both the old dest and src
/// @param dst_ptr A pointer to the dest string pointer
/// @param src The source string to append, it will be freed after operation
void	ft_strcat_free(char **dst_ptr, char *src)
{
	size_t		dst_len;
	size_t		src_len;
	char		*ans;

	dst_len = ft_strlen(*dst_ptr);
	src_len = ft_strlen(src);
	ans = malloc(dst_len + src_len + 1);
	if (ans)
	{
		ft_memcpy(ans, *dst_ptr, dst_len);
		ft_memcpy(ans + dst_len, src, src_len);
		ans[dst_len + src_len] = '\0';
	}
	free(*dst_ptr);
	*dst_ptr = ans;
	free(src);
}

/// @brief Cleans a parameter string by removing redirections parts and skippin
//			over them
/// @param str The raw parameter string to clean
/// @return A newly allocated cleaned version of the input string, or NULL
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

/// @brief Fills the 'params' array with expanded parameters that are parsed
/// @param params The array to store the parsed parameters
/// @param str The full input string to tokenize and expand
/// @param cmd The command structure to update ic of partial allocation fail
/// @param start The current index in the string to start parsing from
/// @return RET_OK on success, or RET_ERR if memory allocation failed
int	construct_params(char **params, char *str, t_cmd *cmd, size_t start)
{
	size_t	end;

	end = 0;
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
			return (free(str), print_error(ERR_MALLOC));
		}
		else if (ft_strlen(params[cmd->nbparams - 1]) == 0)
			free(params[cmd->nbparams-- - 1]);
		start += ++end;
	}
	return (RET_OK);
}

/// @brief Parses a string to extract and expand its parameters, then store
///			it in the parameters array
/// @param str The raw input string to parse
/// @param cmd The command structure in which to store the parameters
/// @return RET_OK on success, RET_ERR on failure
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
