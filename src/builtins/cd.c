/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 12:47:14 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/15 09:53:16 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Function to save the previous pwd path
/// @param void
/// @return set the OLPWD env variable, return 0 on success, 1 on error
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

/// @brief Function to update the PWD path
/// @param void
/// @return Update the PWD env variable, return 0 on success, 1 on error
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

/// @brief
/// @param args
/// @return
static char	*get_target_path(char **args)
{
	char	*path;

	if (!args[1] || !args[1][0])
	{
		path = ft_getenv("HOME");
		if (!path)
			return (fd_printf(STDERR_FILENO, "cd: HOME not set\n"), NULL);
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		path = ft_getenv("OLDPWD");
		if (!path)
			return (fd_printf(STDERR_FILENO, "cd: OLDPWD not set\n"), NULL);
	}
	else
		path = ft_strdup(args[1]);
	return (path);
}

/// @brief
/// @param args
/// @return
int	ft_cd(char **args)
{
	char	*path;
	int		ret;

	path = get_target_path(args);
	if (!path)
		return (RET_ERR);
	ret = save_old_pwd();
	if (ret != 0)
		return (free(path), ret);
	if (chdir(path) != 0)
		return (fd_printf(STDERR_FILENO, "cd: %s: %s\n", path,
				strerror(errno)), free(path), RET_ERR);
	ret = update_pwd();
	return (free(path), ret);
}
