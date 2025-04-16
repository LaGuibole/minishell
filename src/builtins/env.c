/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 12:57:05 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/16 19:48:33 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Builtin implementation of the env command
void	ft_env(void)
{
	t_list	*envp;
	char	*entry;

	envp = *ft_envp(NULL);
	while (envp)
	{
		entry = (char *)envp->content;
		if (ft_strchr(entry, '=') && ft_strlen(ft_strchr(entry, '=')) > 1)
			fd_printf(STDOUT_FILENO, "%s\n", envp->content);
		envp = envp->next;
	}
}
