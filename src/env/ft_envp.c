/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_envp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:38:48 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/05 16:22:05 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Init or retrieve env variables list
/// @param envp Array of strings representing env variables
/// 			if NULL, the function returns saved list
/// @return Pointer to the environment list if envp is NULL, otherwise NULL
t_list	**ft_envp(char **envp)
{
	static t_list	*saved_envp = NULL;
	size_t			i;

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

/// @brief Retrieve the value of an env variable
/// @param name Name of the env variaable to look for (for instance : "PATH")
/// @return A newly allocated copy of variable value or NULL if not found
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

/// @brief Add a new env variable or update an existing one
/// @param name Name of the variable to define or update
/// @param value Value to assign to the variable
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
		if (ft_strncmp(entry, name, name_len) == 0 && entry[name_len] == '='
			&& !ft_strchr(name, '=') && ft_strlen(entry) > name_len)
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

/// @brief Remove an env variable from the list if it exists
/// @param name Name of the variable to delete
void	ft_unsetenv(const char *name)
{
	t_list	**envp;
	t_list	*curr;
	t_list	*prev;
	size_t	len;
	char	*entry;

	if (!name)
		return ;
	len = ft_strlen(name);
	envp = ft_envp(NULL);
	curr = *envp;
	prev = NULL;
	while (curr)
	{
		entry = (char *)curr->content;
		if (match_env_to_delete(entry, name, len))
			return (remove_env_node(envp, curr, prev));
		prev = curr;
		curr = curr->next;
	}
}
