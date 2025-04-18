/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:04:50 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/14 19:07:29 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_sigint(int sig)
{
	(void) sig;
	rl_replace_line("", 0);
	rl_on_new_line();
	write(1, "\n", 1);
	rl_redisplay();
}

int	main(int argc, char **argv, char **envp)
{
	struct sigaction	sa_c;
	(void) argc;
	(void) argv;

	sa_c.sa_handler = handle_sigint;
	sa_c.sa_flags = 0;
	sigemptyset(&sa_c.sa_mask);
	sigaction(SIGINT, &sa_c, NULL);
	signal(SIGQUIT, SIG_IGN);
	ft_envp(envp);
	while (1)
	{
		char *prompt = display_prompt();
		char *line = readline(prompt);
		if (!line)
			return (free(prompt), ft_lstclear(ft_envp(NULL), free), \
					ft_printf("exit\n"),1);
		if (*line)
			add_history(line);
		// ft_pwd();
		if (ft_strlen(line) > 0)
			ft_cd(ft_split(line, ' '));
		free(prompt);
		free(line);
	}
	ft_lstclear(ft_envp(NULL), free);
	return (RET_OK);
}
