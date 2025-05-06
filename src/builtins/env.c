/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 12:57:05 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/06 17:57:05 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Builtin implementation of the env command
int	ft_env(t_exec_ctx *ctx, char **args)
{
	(void)ctx;
	t_list	*envp;
	char	*entry;
	char	*value;

	(void)args;
	envp = *ft_envp(NULL);
	while (envp)
	{
		entry = (char *)envp->content;
		value = ft_strchr(entry, '=');
		if (value)
			fd_printf(STDOUT_FILENO, "%s\n", envp->content);
		envp = envp->next;
	}
	return (RET_OK);
}
