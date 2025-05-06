/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exec_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 10:53:27 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/06 10:54:29 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief horrible function to check if cmd is a builtin
/// @param name the cmd to be checked
/// @return true if *name is a builtin, false otherwise
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
