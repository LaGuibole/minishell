/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:52:41 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/18 11:24:06 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Builtin implementation of the pwd command
/// @return 0 on success, 1 otherwise (getcwd failed)
int	ft_pwd(char **args)
{
	char	*pwd;

	(void)args;
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		fd_printf(STDERR_FILENO, "pwd: getcwd failed\n");
		fd_printf(STDERR_FILENO, strerror(errno));
		return (RET_ERR);
	}
	fd_printf(STDOUT_FILENO, "%s\n", pwd);
	free(pwd);
	return (RET_OK);
}
