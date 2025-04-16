/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 13:10:15 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/16 15:44:26 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Verify if env name variable is valid for export
/// @param name Name of the checked variable
/// @return true is name is valid, false otherwise
bool	is_valid_identifier(const char *name)
{
	int	i;

	if (!name || !*name)
		return (false);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (false);
	i = 1;
	while (name[i] && name[i] != '=')
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

/// @brief Treat arg without equal sign
/// @param arg Arg to be treated
/// @return 0 on success, 1 otherwise
int	process_export_no_equal(char *arg)
{
	char	*value;

	if (!is_valid_identifier(arg))
	{
		print_invalid_identifier(arg);
		return (RET_ERR);
	}
	value = ft_getenv(arg);
	if (value)
		free(value);
	else
		ft_setenv(arg, "");
	return (RET_OK);
}

/// @brief Treat arg with equal sign
/// @param arg Arg to be trated
/// @return 0 on success, 1 otherwise
int	process_export_with_equal(char *arg, char *equal_sign)
{
	char	*name;

	*equal_sign = '\0';
	if (!is_valid_identifier(arg))
	{
		print_invalid_identifier(arg);
		return (RET_ERR);
	}
	name = arg;
	ft_setenv(name, equal_sign + 1);
	return (RET_OK);
}

/// @brief
/// @param sorted_env
void	print_and_free_sorted_env(t_list *sorted_env)
{
	t_list	*current;
	t_list	*tmp;

	current = sorted_env;
	while (current)
	{
		print_export_var((char *)current->content);
		tmp = current;
		current = current->next;
		free(tmp->content);
		free(tmp);
	}
}
/// @brief
/// @param
void	print_sorted_env(void)
{
	t_list	*sorted_env;

	sorted_env = copy_env_list();
	if (!sorted_env)
		return ;
	sort_env_list(sorted_env);
	print_and_free_sorted_env(sorted_env);
}



/// @brief Print the begininng of env variable
/// @param var Variable to display
/// @param equal_sign Pointer to equal sign or NULL
// static void	print_export_var_name(char *var, char *equal_sign)
// {
// 	fd_printf(STDOUT_FILENO, "declare -x ");
// 	if (equal_sign)
// 	{
// 		write(STDOUT_FILENO, var, equal_sign - var);
// 		fd_printf(STDOUT_FILENO, "=\"");
// 	}
// 	else
// 		fd_printf(STDOUT_FILENO, var);
// }

/// @brief Display env variable to export format
/// @param var Variable to display
// static void	print_export_var(char *var)
// {
// 	char	*equal_sign;

// 	equal_sign = ft_strchr(var, '=');
// 	print_export_var_name(var, equal_sign);
// 	if (equal_sign)
// 	{
// 		fd_printf(STDOUT_FILENO, equal_sign + 1);
// 		fd_printf(STDOUT_FILENO, "\"");
// 	}
// 	fd_printf(STDOUT_FILENO, "\n");
// }
