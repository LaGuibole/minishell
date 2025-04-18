/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 10:07:01 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/18 17:11:52 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Check if a builtin should be exec in the parent process(i.e w/ofork)
///	Applies to builtins that affects the shell's state: cd, export, unset, exit
/// @param cmd The command name to check
/// @return true if the command is a parent-executed builtin, false otherwise
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
}
// au dessus : si pas de pipe | si pipe, executer quand meme dans le child

/// @brief Checks if the builtin command can be executed in a child process,
/// such as in a pipe context
/// @param name The command name to check
/// @return true if the builtin can be executed in a pipeline, false otherwise
bool	is_builtin_pipeable(const char *name)
{
	if (ft_strcmp((char *)name, "echo") == 0
		|| ft_strcmp((char *)name, "pwd") == 0
		|| ft_strcmp((char *)name, "env") == 0)
		return (true);
	return (false);
}

/// @brief Executes a builtin command if the cmd matches a known builtin name
///	The builtin table maps cmd names to their corresponding function pointers
/// @param args The command arg array (first element must be the command name)
/// @return The return value of the executed builtin function or RET ERR
int	exec_builtin(t_cmd *cmd)
{
	size_t					i;
	static const t_builtin	builtins[] = {
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
	if (cmd == NULL || cmd->cmd == NULL)
		return (RET_ERR);
	while (builtins[i].name)
	{
		if (ft_strcmp(cmd->cmd, builtins[i].name) == 0)
			return (builtins[i].func(cmd->params));
		i++;
	}
	return (RET_ERR);
}

bool	cmd_is_builtin(char *name)
{
	if (!name)
		return (false);
	if (ft_strcmp(name, "cd") == 0)
		return (true);
	if (ft_strcmp(name, "echo") == 0)
		return (true);
	if (ft_strcmp(name, "env") == 0)
		return (true);
	if (ft_strcmp(name, "exit") == 0)
		return (true);
	if (ft_strcmp(name, "export") == 0)
		return (true);
	if (ft_strcmp(name, "pwd") == 0)
		return (true);
	if (ft_strcmp(name, "unset") == 0)
		return (true);
	return (false);
}
