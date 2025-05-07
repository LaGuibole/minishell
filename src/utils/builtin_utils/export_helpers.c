/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 13:10:15 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/07 13:34:04 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Check if a variable name is a valid identifier for export
/// @param name The name to validate
/// @return true if the name is valid, false otherwise
bool	is_valid_identifier(const char *name)
{
	size_t	i;

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

/// @brief Handle an export argument without '='
/// @param arg The argument to process (e.g, just a variable name)
/// @return 0 on success, 1 otherwise (invalid identifier)
int	process_export_no_equal(char *arg)
{
	if (!is_valid_identifier(arg))
	{
		print_invalid_identifier(arg);
		return (RET_ERR);
	}
	return (RET_OK);
}

/// @brief Handle an export argument containing '='
/// @param arg The full argument string (name=value)
/// @return 0 on success, 1 otherwise (invalid identifier)
int	process_export_with_equal(char *arg, char *equal_sign)
{
	char	*name;
	char	*value;
	char	*tmp;

	if (!is_valid_identifier(arg))
	{
		print_invalid_identifier(arg);
		return (RET_ERR);
	}
	tmp = ft_strdup(arg);
	if (!tmp)
		return (RET_ERR);
	equal_sign = ft_strchr(tmp, '=');
	*equal_sign = '\0';
	name = tmp;
	value = equal_sign + 1;
	ft_setenv(name, value);
	free(tmp);
	return (RET_OK);
}

/// @brief Print the env variables in export format and free the list
/// @param sorted_env The sorted environement list to display and free
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

/// @brief Copy, sort and print the environment in export format
void	print_sorted_env(void)
{
	t_list	*sorted_env;

	sorted_env = copy_env_list();
	if (!sorted_env)
		return ;
	sort_env_list(sorted_env);
	print_and_free_sorted_env(sorted_env);
}
