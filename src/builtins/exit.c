/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 20:46:08 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/07 17:51:08 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			free_all_and_exit(ctx->head, 255);
		}
		status = ft_atoi(args[1]);
	}
	free_all_and_exit(ctx->head, status);
	return (0);
}

// int	ft_exit(t_cmd *cmd, char **args)
// {
// 	int	code;

// 	write(2, "exit\n", 5);
// 	if (!args[1])
// 		free_all_and_exit(cmd, 0);
// 	if (!is_numeric(args[1]))
// 	{
// 		ft_putstr_fd("minishell: exit: ", 2);
// 		ft_putstr_fd(args[1], 2);
// 		ft_putstr_fd(": numeric argument required\n", 2);
// 		free_all_and_exit(cmd, 255);
// 	}
// 	if (args[2])
// 	{
// 		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
// 		return (1);
// 	}
// 	code = ft_atoi(args[1]);
// 	free_all_and_exit(cmd, code % 256);
// 	return (0);
// }
