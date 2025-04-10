/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_prompt.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:59:19 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/09 14:31:03 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*replace_home_with_tilde(char *path)
{
	char	*home;
	size_t	home_len;
	char	*result;

	if (!path)
		return (NULL);
	home = ft_getenv("HOME");
	if (!home)
		return (ft_strdup(path));
	home_len = ft_strlen(home);
	if (ft_strncmp(path, home, home_len) == 0 && (path[home_len] == '/' || path[home_len] == '\0'))
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
