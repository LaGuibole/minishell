/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 13:10:15 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/15 18:09:00 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Verify if env name variable is valid for export
/// @param name Name of the checked variable
/// @return true is name is valid, false otherwise
static bool	is_valid_identifier(const char *name)
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

/// @brief Print error message for invalid identifier
/// @param arg Invalid arg
static void	print_invalid_identifier(char *arg)
{
	fd_printf(STDERR_FILENO, "minishell: export: `%s': not a valid identifier\n", arg);
}

/// @brief Treat arg without equal sign
/// @param arg Arg to be treated
/// @return 0 on success, 1 otherwise
static int	process_export_no_equal(char *arg)
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
static int	process_export_with_equal(char *arg, char *equal_sign)
{
	char	*name;
	char	*value;
	char	tmp;

	tmp = *equal_sign;
	*equal_sign = '\0';
	if (!is_valid_identifier(arg))
	{
		*equal_sign = tmp;
		print_invalid_identifier(arg);
		return (RET_ERR);
	}
	name = arg;
	*equal_sign = tmp;
	value = equal_sign + 1;
	ft_setenv(name, value);
	return (RET_OK);
}

/// @brief Treat export arg
/// @param arg Arg to be treated
/// @return 0 on success, 1 otherwise
static int	process_export_arg(char *arg)
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

/// @brief Print the begininng of env variable
/// @param var Variable to display
/// @param equal_sign Pointer to equal sign or NULL
static void	print_export_var_name(char *var, char *equal_sign)
{
	fd_printf(STDOUT_FILENO, "declare -x ");
	if (equal_sign)
	{
		write(STDOUT_FILENO, var, equal_sign - var);
		fd_printf(STDOUT_FILENO, "=\"");
	}
	else
		fd_printf(STDOUT_FILENO, var);
}

/// @brief Display env variable to export format
/// @param var Variable to display
static void	print_export_var(char *var)
{
	char	*equal_sign;

	equal_sign = ft_strchr(var, '=');
	print_export_var_name(var, equal_sign);
	if (equal_sign)
	{
		fd_printf(STDOUT_FILENO, equal_sign + 1);
		fd_printf(STDOUT_FILENO, "\"");
	}
	fd_printf(STDOUT_FILENO, "\n");
}

/// @brief Exchange helper for sorting
/// @param a Pointer to first element
/// @param b Pointer to second element
static void	swap_nodes(t_list *a, t_list *b)
{
	void	*tmp;

	tmp = a->content;
	a->content = b->content;
	b->content = tmp;
}

/// @brief Copy env lsit
/// @return Copied env list
static t_list	*copy_env_list(void)
{
	t_list	*envp;
	t_list	*sorted_env;
	t_list	*new_node;

	sorted_env = NULL;
	envp = *ft_envp(NULL);
	while (envp)
	{
		new_node = ft_lstnew(ft_strdup((char *)envp->content));
		if (!new_node)
		{
			ft_lstclear(&sorted_env, free);
			return (NULL);
		}
		ft_lstadd_back(&sorted_env, new_node);
		envp = envp->next;
	}
	return (sorted_env);
}

static int	sort_env_list_part(t_list *sorted_env, t_list **current, t_list *last)
{
	int swapped;

	swapped = 0;
	*current = sorted_env;
	while ((*current)->next != last)
	{
		if (ft_strcmp((char *)(* current)->content, (char *)(*current)->next->content) > 0)
		{
			swap_nodes(*current, (*current)->next);
			swapped = 1;
		}
		*current = (*current)->next;
	}
	return (swapped);
}

/// @brief Sort env list (bubble sort)
/// @param sorted_env List to be sorted
static void	sort_env_list(t_list *sorted_env)
{
	t_list	*current;
	t_list	*last;
	int		swapped;

	if (!sorted_env)
		return ;
	last = NULL;
	swapped = 1;
	while (swapped)
	{
		swapped = sort_env_list_part(sorted_env, &current, last);
		last = current;
	}
}
static void	print_and_free_sorted_env(t_list *sorted_env)
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

static void	print_sorted_env(void)
{
	t_list	*sorted_env;

	sorted_env = copy_env_list();
	if (!sorted_env)
		return ;
	sort_env_list(sorted_env);
	print_and_free_sorted_env(sorted_env);
}

static int	process_export_args(char **args)
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

int	ft_export(char **args)
{
	if (!args[1])
	{
		print_sorted_env();
		return (RET_OK);
	}
	return (process_export_args(args));
}




