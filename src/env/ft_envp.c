/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_envp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:38:48 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/09 15:04:43 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (ft_strncmp(entry, name, name_len) == 0 && entry[name_len] == '=')
		{
			free(envp->content);
			new_entry = ft_strjoin(ft_strjoin(name, "="), value); // a free
			envp->content = new_entry;
			return ;
		}
		envp = envp->next;
	}
	new_entry = ft_strjoin(ft_strjoin(name, "="), value);
	ft_lstadd_back(ft_envp(NULL), ft_lstnew(new_entry));
}

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
