/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mktemp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:06:57 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/08 19:17:53 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Construct a temporary filename based on the process ID and a counter
/// @param pid The process ID used to build the name
/// @param count A counter used to ensure uniqueness
/// @return A newly allocated string containing the full temp path |NULL (error)
static char	*build_tmp_name(size_t rand, int count)
{
	char	*id;
	char	*full;

	id = ft_itoa(rand + count);
	if (!id)
		return (NULL);
	full = free_join("/tmp/heredoc_", id, false, true);
	return (full);
}

/// @brief Generate a unique temp filename in /tmp for heredoc usage
/// @return A newly allocated string containing a unique filename | NULL(error)
char	*ft_mktemp(bool res)
{
	static size_t	rand = 1;
	char			*tmp;
	char			*result;

	if (res)
		return (ft_itoa((int)rand));
	tmp = build_tmp_name(rand, 0);
	rand++;
	result = ft_strdup(tmp);
	free(tmp);
	return (result);
}
