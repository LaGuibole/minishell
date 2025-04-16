/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_envp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:38:48 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/16 14:12:54 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Init or store linked list representing env variables
/// @param envp String array representing env variables (NULL to store existing list)
/// @return Double pointer to the linked list of env variables if envp is NULL, NULL otherwise
t_list	**ft_envp(char **envp)
{
	size_t			i;
	static t_list	*saved_envp = NULL;

	i = 0;
	if (!envp)
	{
		return (&saved_envp);
	}
	while (envp[i])
	{
		ft_lstadd_back(&saved_envp, ft_lstnew(ft_strdup(envp[i])));
		i++;
	}
	return (NULL);
}

/// @brief Store given env variable into linked list
/// @param name Name of the env variaable to look for (for instance : "PATH")
/// @return A copy of the associated value, NULL if not found
char	*ft_getenv(const char *name)
{
	t_list	*envp;
	size_t	name_len;
	char	*entry;

	if (!name)
		return (NULL);
	name_len = ft_strlen(name);
	envp = *ft_envp(NULL);
	while (envp)
	{
		entry = (char *)envp->content;
		if (ft_strncmp(entry, name, name_len) == 0 && entry[name_len] == '=')
			return (ft_strdup(entry + name_len + 1));
		envp = envp->next;
	}
	return (NULL);
}

/// @brief Add or modify an env variable
/// @param name Name of the variable to define
/// @param value Value to associate to the variable
void	ft_setenv(const char *name, const char *value)
{
	t_list	*envp;
	char	*entry;
	char	*new_entry;
	size_t	name_len;

	if (!name || !value)
		return ;
	name_len = ft_strlen(name);
	envp = *ft_envp(NULL);
	while (envp)
	{
		entry = (char *)envp->content;
		if (ft_strncmp(entry, name, name_len) == 0 && entry[name_len] == '=' && !ft_strchr(name, '=') && ft_strlen(entry) > name_len)
		{
			free(entry);
			new_entry = ft_strjoin(name, "=");
			envp->content = free_join(new_entry, (char *)value, true, false);
			return ;
		}
		envp = envp->next;
	}
	new_entry = ft_strjoin(name, "=");
	new_entry = free_join(new_entry, (char *)value, true, false);
	ft_lstadd_back(ft_envp(NULL), ft_lstnew(new_entry));
}

/// @brief Delete an env variable if it exists
/// @param name Name of the variable to delete
void	ft_unsetenv(const char *name)
{
	t_list	**envp_head;
	t_list	*curr;
	t_list	*prev;
	size_t	name_len;
	char	*entry;

	if (!name)
		return ;
	name_len = ft_strlen(name);
	envp_head = ft_envp(NULL);
	curr = *envp_head;
	prev = NULL;
	while (curr)
	{
		entry = (char *)curr->content;
		if (ft_strncmp(entry, name, name_len) == 0 && entry[name_len] == '=')
		{
			if (prev)
				prev->next = curr->next;
			else
				*envp_head = curr->next;
			free(curr->content);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}
