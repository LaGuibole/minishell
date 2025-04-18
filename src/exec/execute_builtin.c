/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 10:07:01 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/18 10:47:04 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief
/// @param cmd
/// @return
bool	is_parent_builtin(char *cmd)
{
	if (!cmd)
		return (false);
	if (ft_strcmp(cmd, "cd") == 0)
		return (true);
	if (ft_strcmp(cmd, "export") == 0)
		return (true);
	if (ft_strcmp(cmd, "unset") == 0)
		return (true);
	if (ft_strcmp(cmd, "exit") == 0)
		return (true);
	return (false);
} // si pas de pipe | si pipe, executer quand meme dans le child

/// @brief
/// @param args
/// @return
int	exec_builtin(char **args)
{
	size_t	i;

	static const t_builtin builtins[] = {
		{"cd", &ft_cd},
		{"echo", &ft_echo},
		{"env", &ft_env},
		//{"exit", &ft_exit},
		{"export", &ft_export},
		{"pwd", &ft_pwd},
		{"unset", &ft_unset},
		{NULL, NULL}
	};

	i = 0;
	if (args == NULL || args[0] == NULL)
		return (RET_ERR);
	while (builtins[i].name)
	{
		if (ft_strcmp(args[0], builtins[i].name) == 0)
			return (builtins[i].func(args));
		i++;
	}
	return (RET_ERR);
}
