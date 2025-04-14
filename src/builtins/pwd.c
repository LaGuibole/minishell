/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:52:41 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/14 12:21:37 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		fd_printf(STDERR_FILENO, "pwd: getced failed\n");
		fd_printf(STDERR_FILENO, strerror(errno));
		return (1);
	}
	fd_printf(STDOUT_FILENO, pwd);
	fd_printf(STDOUT_FILENO, "\n");
	free(pwd);
	return (0);
}
