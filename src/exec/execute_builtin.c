/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 10:07:01 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/06 17:58:28 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"

/// @brief Executes a builtin command if the cmd matches a known builtin name
///	The builtin table maps cmd names to their corresponding function pointers
/// @param args The command arg array (first element must be the command name)
/// @return The return value of the executed builtin function or RET ERR
int	exec_builtin(t_exec_ctx *ctx)
{
	size_t					i;
	static const t_builtin	builtins[] = {
	{"cd", &ft_cd},
	{"echo", &ft_echo},
	{"env", &ft_env},
	{"exit", &ft_exit},
	{"export", &ft_export},
	{"pwd", &ft_pwd},
	{"unset", &ft_unset},
	{NULL, NULL}
	};

	i = 0;
	if (ctx->curr == NULL || ctx->curr->cmd == NULL)
		return (RET_ERR);
	while (builtins[i].name)
	{
		if (ft_strcmp(ctx->curr->cmd, builtins[i].name) == 0)
			return (builtins[i].func(ctx, ctx->curr->params));
		i++;
	}
	return (RET_ERR);
}
