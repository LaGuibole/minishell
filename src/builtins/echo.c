/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:07:03 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/09 11:07:29 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Check if the given argument is a valid -n flag
/// @param arg The argument to check (eg, "-n", "-nnn" etc)
/// @return true of the arg is a valid -n option, false otherwise
static bool	is_n_flag(char *arg)
{
	size_t	i;

	if (!arg || arg[0] != '-' || ft_strlen(arg) < 2)
		return (false);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

/// @brief Builtin implementation of the echo command
/// @param args The argument list passed to echo
/// @return 0 always (RET_OK)
int	ft_echo(t_exec_ctx *ctx, char **args)
{
	bool	n_option;

	(void)ctx;
	n_option = false;
	args++;
	while (*args && is_n_flag(*args))
	{
		n_option = true;
		args++;
	}
	while (*args)
	{
		fd_printf(STDOUT_FILENO, "%s", *args++);
		if (*args)
			fd_printf(STDOUT_FILENO, " ");
	}
	if (!n_option)
		fd_printf(STDOUT_FILENO, "\n");
	return (RET_OK);
}
