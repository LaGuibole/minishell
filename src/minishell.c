/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:04:50 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/30 17:18:59 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_signal = 0;

void	handle_sigint(int sig)
{
	(void) sig;
	g_signal = 1;
	rl_replace_line("", 0);
	rl_on_new_line();
	write(1, "\n", 1);
	rl_redisplay();
}

int	main(int argc, char **argv, char **envp)
{
	struct sigaction	sa_c;

	(void)argc;
	(void)argv;
	sa_c.sa_handler = handle_sigint;
	sa_c.sa_flags = 0;
	sigemptyset(&sa_c.sa_mask);
	sigaction(SIGINT, &sa_c, NULL);
	signal(SIGQUIT, SIG_IGN);
	ft_envp(envp);
	while (1)
	{
		t_cmd	*cmds;
		char	*prompt = display_prompt();
		char	*line = readline(prompt);
		free(prompt);
		if (!line)
			return (ft_lstclear(ft_envp(NULL), free), \
					ft_printf("exit\n"), 1);
		if (line)
			add_history(line);
		if (ft_strlen(line) == 0)
			continue ;
		cmds = parsing_cmd(line);
		if (cmds)
		{
			// print_cmd_list(cmds);
			exec_pipeline(cmds, envp);
			free_cmd_list(cmds);
		}
		free(line);
	}
	ft_lstclear(ft_envp(NULL), free);
	return (0);
}
