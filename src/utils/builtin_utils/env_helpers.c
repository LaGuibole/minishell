/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:26:46 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/16 15:49:30 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief
/// @param entry
/// @param name
/// @return
bool	match_env_entry(const char *entry, const char *name)
{
	size_t	len;

	if (!entry || !name)
		return (false);
	len = ft_strlen(name);
	if (ft_strchr(name,  '='))
		return (false);
	if (ft_strncmp(entry, name, len) != 0)
		return (false);
	if (entry[len] != '=')
		return(false);
	return (true);
}

/// @brief Exchange helper for sorting
/// @param a Pointer to first element
/// @param b Pointer to second element
void	swap_nodes(t_list *a, t_list *b)
{
	void	*tmp;

	tmp = a->content;
	a->content = b->content;
	b->content = tmp;
}

/// @brief Copy env lsit
/// @return Copied env list
t_list	*copy_env_list(void)
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

/// @brief
/// @param sorted_env
/// @param current
/// @param last
/// @return
int	sort_env_list_part(t_list *sorted_env, t_list **current, t_list *last)
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
void	sort_env_list(t_list *sorted_env)
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
