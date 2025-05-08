/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 12:47:14 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/08 22:12:36 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Save the current workind directory to OLDPWD
/// @return 0 on success, 1 otherwise (eg ; getcwd failed)
static int	save_old_pwd(void)
{
	char	*old_pwd;

	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
	{
		fd_printf(STDERR_FILENO, "cd: getcwd failed: %s\n", strerror(errno));
		return (RET_ERR);
	}
	ft_setenv("OLDPWD", old_pwd);
	free(old_pwd);
	return (RET_OK);
}

/// @brief Update the current working directory in PWD
/// @return 0 on success, 1 otherwise (eg ; getcwd failed)
static int	update_pwd(void)
{
	char	*new_pwd;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		fd_printf(STDERR_FILENO, "cd: getcwd failed: %s\n", strerror(errno));
		return (RET_ERR);
	}
	ft_setenv("PWD", new_pwd);
	free(new_pwd);
	return (RET_OK);
}

/// @brief Determine the target path for the 'cd' command based on arguments.
///			Supports HOME (no arg or ~ or --) and OLDPWD
/// @param args The arguments array passed to the 'cd' builtin
/// @return A newly allocated string representing the resolved path
///			NULL if resolution fails
static char	*get_target_path(char **args)
{
	char	*path;

	path = NULL;
	if (!args[1])
		path = get_home_path();
	else if (ft_strcmp(args[1], "~") == 0 || ft_strcmp(args[1], "--") == 0)
		path = get_home_path();
	else if (ft_strcmp(args[1], "-") == 0)
		path = get_oldpwd_path();
	else
		path = ft_strdup(args[1]);
	return (path);
}

/// @brief Executes the 'cd' builtin. Handles HOME, OLDPWD, updates
///			env var and prints error message if needed.
/// @param ctx The execution context (currently unsused).
/// @param args The arg array (expected: args[0] = 'cd', args[1] = target)
/// @return RET_OK (0) on success, RET_ERR (-1) on failure
int	ft_cd(t_exec_ctx *ctx, char **args)
{
	char	*path;
	int		ret;

	(void)ctx;
	path = get_target_path(args);
	if (!path)
		return (RET_ERR);
	ret = save_old_pwd();
	if (ret != 0)
	{
		free(path);
		return (ret);
	}
	if (chdir(path) != 0)
	{
		fd_printf(STDERR_FILENO, "cd: %s: %s\n", path, strerror(errno));
		g_signal = 2;
		free(path);
		return (RET_ERR);
	}
	ret = update_pwd();
	free(path);
	return (ret);
}
