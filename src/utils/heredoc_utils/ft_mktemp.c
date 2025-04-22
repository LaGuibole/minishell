/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mktemp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:06:57 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/18 11:33:48 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*build_tmp_name(pid_t pid, int count)
{
	char	*id;
	char	*full;

	id = ft_itoa(pid + count);
	if (!id)
		return (NULL);
	full = free_join("/tmp/heredoc_", id, false, true);
	return (full);
}

char	*ft_mktemp(void)
{
	static size_t	count;
	pid_t			pid;
	char			*tmp;
	char			*result;

	count = 0;
	pid = getpid();
	tmp = build_tmp_name(pid, count);
	while (tmp && access(tmp, F_OK) == 0)
	{
		free(tmp);
		count++;
		tmp = build_tmp_name(pid, count);
	}
	result = ft_strdup(tmp);
	free(tmp);
	return (result);
}
