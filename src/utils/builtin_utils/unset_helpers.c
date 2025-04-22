/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 18:46:24 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/16 19:45:03 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Check if an environment entry matches the name to delete
/// @param entry The environment variable entry (e.g., "VAR=value")
/// @param name The name to match (e.g., "VAR")
/// @param len The length of the name
/// @return true if the entry matches the variable name, false otherwise
bool	match_env_to_delete(char *entry, const char *name, size_t len)
{
	if (ft_strncmp(entry, name, len) == 0 && entry[len] == '=')
		return (true);
	return (false);
}

/// @brief Remove a node from the environment linked list
/// @param head Pointer to the head of the environment list
/// @param curr The node to remove
/// @param prev The node before the one to remove, or NULL if removing the head
void	remove_env_node(t_list **head, t_list *curr, t_list *prev)
{
	if (prev)
		prev->next = curr->next;
	else
		*head = curr->next;
	free(curr->content);
	free(curr);
}
