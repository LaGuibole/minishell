/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:04:50 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/09 15:11:44 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void) argc;
	(void) argv;
	signal(SIGQUIT, SIG_IGN);
	ft_envp(envp);
	while (1)
	{
		char *prompt = display_prompt();
		char *line = readline(prompt);
		if (!line)
			return (free(prompt), ft_lstclear(ft_envp(NULL), free), 1);
		if (*line)
			add_history(line);
		free(prompt);
		free(line);
	}
	ft_lstclear(ft_envp(NULL), free);
	return (0);
}
