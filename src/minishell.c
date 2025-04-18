/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:04:50 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/18 13:57:02 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint(int sig)
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
	int heredoc_fd = -1;
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
		char **args = split_free(line, ' ', false);
		if (*line)
			add_history(line);
		exec_builtin(args);
		if (args && args[0] && ft_strncmp(args[0], "heredoc", 8) == 0)
			heredoc_fd = create_heredoc_fd("EOF");
		free_double_tab(args);
		free(prompt);
		free(line);
		if (heredoc_fd != -1)
			close(heredoc_fd);
	}
	ft_lstclear(ft_envp(NULL), free);
	return (RET_OK);
}
