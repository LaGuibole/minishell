/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 20:46:08 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/08 22:36:35 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Check whether a string contains only numerix characters (0-9)
/// @param arg The string to check
/// @return true if the string is numeric, false otherwise
static bool	is_numeric(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (false);
		i++;
	}
	return (true);
}

/// @brief Executes the 'exit' builtin. Handles numeric argument,
///			Error cases, and frees all memory before exiting the shell
/// @param ctx The execution context containing the cmd list to free
/// @param args The arg array (expected arg[0] = 'exit', arg[1]= optional num)
/// @return This function only returns if there are too many arg
///			Otherwise it terminates the shell with the specified status
int	ft_exit(t_exec_ctx *ctx, char **args)
{
	int	status;

	status = 0;
	ft_lstclear(ft_envp(NULL), free);
	if (args && args[1])
	{
		if (args[2])
		{
			fd_printf(STDERR_FILENO, "exit: too many arguments\n");
			return (ft_atoi(args[1]));
		}
		if (!is_numeric(args[1]))
		{
			fd_printf(STDERR_FILENO, "exit: error, arg must be an int\n");
			free_all_and_exit(ctx->head, 2);
		}
		status = ft_atoi(args[1]);
	}
	free_all_and_exit(ctx->head, status);
	return (0);
}
