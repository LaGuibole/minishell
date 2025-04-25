/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_helpers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 21:46:46 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/23 21:58:15 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_home_path(void)
{
	char	*home;
	char	*copy;

	home = ft_getenv("HOME");
	if (!home)
	{
		fd_printf(STDERR_FILENO, "cd: HOME not set\n");
		return (NULL);
	}
	if (home[0] == '\0')
		return (NULL);
	copy = ft_strdup(home);
	return (copy);
}

char	*get_oldpwd_path(void)
{
	char	*oldpwd;
	char	*copy;

	oldpwd = ft_getenv("OLDPWD");
	if (!oldpwd)
	{
		fd_printf(STDERR_FILENO, "cd: OLDPWD not set\n");
		return (NULL);
	}
	if (oldpwd[0] == '\0')
		return (NULL);
	fd_printf(STDOUT_FILENO, "%s\n", oldpwd);
	copy = ft_strdup(oldpwd);
	return (copy);
}
