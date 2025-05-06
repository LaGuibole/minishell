/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:04:50 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/06 17:48:50 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_signal = 0;

void	handle_signal_interactive(int sig)
{
	if (sig == SIGINT)
		g_signal = 130;
	else
		g_signal = sig;
	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_signal_executing(int sig)
{
	if (sig == SIGQUIT)
		ft_putstr_fd("Quit (core dumped)\n", STDOUT_FILENO);
	else if (sig == SIGINT)
	{
		g_signal = 130;
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
	else
		g_signal = sig;
}



int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	ft_envp(envp);
	while (1)
	{
		t_cmd	*cmds;

		signal(SIGINT, handle_signal_interactive);
		signal(SIGQUIT, SIG_IGN);
		char	*prompt = display_prompt();
		char	*line = readline(prompt);
		free(prompt);
		if (!line)
			return (ft_lstclear(ft_envp(NULL), free), \
					ft_printf("exit\n"), g_signal);
		if (line)
			add_history(line);
		if (ft_strlen(line) == 0)
			continue ;
		cmds = parsing_cmd(line);
		if (cmds)
		{
			// print_cmd_list(cmds);
			exec_pipeline(cmds);
			free_cmd_list(cmds);
		}
		free(line);
	}
	ft_lstclear(ft_envp(NULL), free);
	return (g_signal);
}
