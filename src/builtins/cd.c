/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 12:47:14 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/14 16:08:52 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(char **args)
{
	char	*path;
	char	*old_pwd;
	char	*new_pwd;

	if (!args[1] || !args[1][0])
	{
		path = ft_getenv("HOME");
		if (!path)
		{
			fd_printf(STDERR_FILENO, "cd: HOME not set\n");
			return (1);
		}
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		path = ft_getenv("OLDPWD");
		if (!path)
		{
			fd_printf(STDERR_FILENO, "cd: OLDPWD not set\n");
			return (1);
		}
		// fd_printf(STDOUT_FILENO, path);
	}
	else
		path = ft_strdup(args[1]);
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
	{
		fd_printf(STDERR_FILENO, "cd: getcwd failed\n");
		free(path);
		return (1);
	}
	if (chdir(path) != 0)
	{
		fd_printf(STDERR_FILENO, "cd: ");
		fd_printf(STDERR_FILENO, path);
		fd_printf(STDERR_FILENO, ": ");
		fd_printf(STDERR_FILENO, strerror(errno));
		fd_printf(STDERR_FILENO, "\n");
		free(path);
		free(old_pwd);
		return (1);
	}
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		fd_printf(STDERR_FILENO, "cd: getcwd failed\n");
		free(path);
		free(old_pwd);
		return (1);
	}
	ft_setenv("OLDPWD", old_pwd);
	ft_setenv("PWD", new_pwd);
	free(path);
	free(old_pwd);
	free(new_pwd);
	return (0);
}
