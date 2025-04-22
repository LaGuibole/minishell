/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 13:53:33 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/18 15:56:34 by guphilip         ###   ########.fr       */
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

/// @brief Retrieves the value of the PATH environment variable
/// @param envp The environment variables array
/// @return A pointer to the string containing the PATH value,
///			(excluding "PATH="), or NULL if not found
char	*get_env_path(char **envp)
{
	size_t	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == RET_OK)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

/// @brief Splits the PATH envrionment variable into an array of directory path
/// @param envp The envv variable array
/// @return A NULL-terminated array of strings, each being a directory in PATH
///			NULL if PATH is not found
char	**get_paths(char **envp)
{
	char	*path_env;

	path_env = get_env_path(envp);
	if (!path_env)
		return (NULL);
	return (ft_split(path_env, ':'));
}

/// @brief Joins a directory path and a command to create a full exec path
/// @param path The directory path (e.g: "/bin")
/// @param cmd The command name (e.g: "ls")
/// @return A newly allocated string containing the full path (e.g: "/bin/ls")
///			or NULL on failure
char	*join_cmd_paths(char *path, char *cmd)
{
	char	*tmp;
	char	*full_path;

	tmp = ft_strjoin(path, "/");
	if (!tmp)
		return (NULL);
	full_path = free_join(tmp, cmd, true, false);
	if (!full_path)
		return (NULL);
	return (full_path);
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
