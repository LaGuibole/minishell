/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:10:19 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/16 15:58:02 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Treat export arg
/// @param arg Arg to be treated
/// @return 0 on success, 1 otherwise
int	process_export_arg(char *arg)
{
	char	*equal_sign;

	if (!arg || !*arg)
		return (RET_ERR);
	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
		return (process_export_no_equal(arg));
	else
		return (process_export_with_equal(arg, equal_sign));
}

int	process_export_args(char **args)
{
	int	i;
	int	status;

	status = 0;
	i = 1;
	while (args[i])
	{
		if (process_export_arg(args[i]) != 0)
			status = 1;
		i++;
	}
	return (status);
}

void	print_export_var(char *var)
{
	char	*equal_sign;

	equal_sign = ft_strchr(var, '=');
	fd_printf(STDOUT_FILENO, "declare -x ");

	if (equal_sign)
	{
		write(STDOUT_FILENO, var, equal_sign - var);
		fd_printf(STDOUT_FILENO, "=\"");
		fd_printf(STDOUT_FILENO, equal_sign + 1);
		fd_printf(STDOUT_FILENO, "\"");
	}
	else
		fd_printf(STDOUT_FILENO, var);
	fd_printf(STDOUT_FILENO, "\n");
}

int	ft_export(char **args)
{
	if (!args[1])
	{
		print_sorted_env();
		return (RET_OK);
	}
	return (process_export_args(args));
}
