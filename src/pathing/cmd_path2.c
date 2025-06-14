/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 14:23:57 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/22 14:24:33 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
