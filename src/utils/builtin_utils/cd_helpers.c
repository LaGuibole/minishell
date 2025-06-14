/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_helpers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 21:46:46 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/08 21:20:52 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Retrieves and duplicates the value of the HOME environment variable
///		Prints an error if HOME is not set or is empty
/// @return A newly allocated copy of the HOME path, or NULL on failure
char	*get_home_path(void)
{
	char	*home;
	char	*copy;

	home = ft_getenv("HOME");
	if (!home)
	{
		fd_printf(STDERR_FILENO, "cd: HOME not set\n");
		return (NULL);
	}
	if (home[0] == '\0')
	{
		free(home);
		return (NULL);
	}
	copy = ft_strdup(home);
	free(home);
	return (copy);
}

/// @brief Retrieves and duplicates the value of the OLDPWD env var
///			Prints an error and the value to STDOUT if available
/// @return	A newly allocated copy of the OLDPWD path, or NULL on failure
char	*get_oldpwd_path(void)
{
	char	*oldpwd;
	char	*copy;

	oldpwd = ft_getenv("OLDPWD");
	if (!oldpwd)
	{
		fd_printf(STDERR_FILENO, "cd: OLDPWD not set\n");
		return (NULL);
	}
	if (oldpwd[0] == '\0')
	{
		free(oldpwd);
		return (NULL);
	}
	fd_printf(STDOUT_FILENO, "%s\n", oldpwd);
	copy = ft_strdup(oldpwd);
	free(oldpwd);
	return (copy);
}
