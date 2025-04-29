/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 13:53:33 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/25 21:49:33 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Checks if a command contains a '/' char, meaning it's a path
/// @param cmd The command to check
/// @return true if the command is a path, false otherwise
static bool	is_path(char *cmd)
{
	size_t	i;

	if (!cmd)
		return (false);
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '/')
			return (true);
		i++;
	}
	return (false);
}

/// @brief Searches for the command in a list of directories (from PATH)
/// @param paths The array of directory paths (from get_paths())
/// @param cmd THe command name to search for
/// @return A newly allocated full path to the executable if found, NULL otherw
/// 		Frees the paths array before returning
static char	*search_in_paths(char **paths, char *cmd)
{
	char	*full_path;
	size_t	i;

	i = 0;
	while (paths[i])
	{
		full_path = join_cmd_paths(paths[i], cmd);
		if (full_path && access(full_path, X_OK) == RET_OK)
		{
			free_double_tab(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_double_tab(paths);
	return (NULL);
}

/// @brief Determines the full path to a command based on whether it's a path
///			or should be searched in PATH
/// @param cmd The command structure (t_cmd) containing the command name
/// @param envp The environement variables array
/// @return A newly allocated string containing the full path to the exec,
///			NULL if not found
char	*get_cmd_path(t_cmd *cmd, char **envp)
{
	char	**paths;

	if (!cmd || !cmd->cmd || !envp)
		return (NULL);
	if (is_path(cmd->cmd) == true)
	{
		if (access(cmd->cmd, X_OK) == RET_OK)
			return (ft_strdup(cmd->cmd));
		return (NULL);
	}
	paths = get_paths(envp);
	if (!paths)
		return (NULL);
	return (search_in_paths(paths, cmd->cmd));
}
