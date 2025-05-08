/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_array.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:33:22 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/08 20:30:41 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Frees a null-terminated array of strings and sets each pointer NULL
/// @param array The array to free. If NULL, the function does nothing
void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
	array = NULL;
}

/// @brief Converts env var list into null-term array of strings
/// @return A newly allocated array of strings representing env var or NULL
char	**env_to_array(void)
{
	t_list	*env;
	int		i;
	char	**array;
	int		size;

	env = *ft_envp(NULL);
	size = ft_lstsize(env);
	array = malloc(sizeof(char *) * (size + 1));
	if (!array)
		return (NULL);
	i = 0;
	while (env)
	{
		array[i] = ft_strdup((char *)env->content);
		if (!array[i])
			return (free_array(array), NULL);
		env = env->next;
		i++;
	}
	array[i] = NULL;
	return (array);
}
