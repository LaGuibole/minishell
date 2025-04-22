/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:10:19 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/16 19:24:03 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Process a single argument passed to the export builtin
/// @param arg The argument to process (can be with or without (youuuuu) '=')
/// @return 0 on success, 1 on error (e.g, invalid identifier)
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

/// @brief Process all arguments passed to the export builtin
/// @param args The argument list (args[0] is "export", [1...] are variables)
/// @return 0 if all succeeded, 1 if at least one arg caused an error
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

/// @brief Print an environment variable in export format
/// @param var The variable to print (e.g, "VAR=value" or "VAR")
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

/// @brief Builtin implementation of the export command
/// @param args The argument list passed to export
/// @return 0 on success, 1 otherwise
int	ft_export(char **args)
{
	if (!args[1])
	{
		print_sorted_env();
		return (RET_OK);
	}
	return (process_export_args(args));
}
