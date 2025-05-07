/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_prompt.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:59:19 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/05 16:28:41 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Replace the beginning of a path with '~' if it starts with $HOME
/// @param path Absolute path to check and maybe modify
/// @return A new allocated string with '~' instead of '$HOME', or path duplica
static char	*replace_home_with_tilde(char *path)
{
	char	*home;
	char	*result;
	size_t	home_len;

	if (!path)
		return (NULL);
	home = ft_getenv("HOME");
	if (!home || home[0] == '\0')
	{
		free(home);
		return (ft_strdup(path));
	}
	home_len = ft_strlen(home);
	if (ft_strncmp(path, home, home_len) == 0
		&& (path[home_len] == '/' || path[home_len] == '\0'))
	{
		if (path[home_len] == '\0')
			result = ft_strdup("~");
		else
			result = ft_strjoin("~", path + home_len);
		free(home);
		return (result);
	}
	free(home);
	return (ft_strdup(path));
}

/// @brief Generate CLI for the shell
/// @return A formated string "~/repo$" or "minishell$" if an error occurs
char	*display_prompt(void)
{
	char	*cwd;
	char	*tilde_path;
	char	*prompt;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (ft_strdup("minishell$ "));
	tilde_path = replace_home_with_tilde(cwd);
	free(cwd);
	prompt = ft_strjoin(tilde_path, "$ ");
	free(tilde_path);
	return (prompt);
}
