/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:04:50 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/08 22:44:10 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

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

void	handle_input(char *input)
{
	t_cmd		*cmds;
	t_exec_ctx	ctx;

	cmds = parsing_cmd(input);
	if (!cmds)
		return (free(input));
	if (cmds)
	{
		print_cmd_list(cmds);
		ctx = (t_exec_ctx){cmds, cmds};
		exec_pipeline(&ctx);
		free_cmd_list(ctx.head);
	}
	free(input);
}

int	main(int argc, char **argv, char **envp)
{
	char		*prompt;
	char		*line;
	int			i;

	(void)argc, (void)argv;
	ft_envp(envp);
	while (1)
	{
		signal(SIGINT, handle_signal_interactive);
		signal(SIGQUIT, SIG_IGN);
		prompt = display_prompt();
		line = readline(prompt);
		free(prompt);
		if (!line)
		{
			line = ft_mktemp(1);
			i = ft_atoi(line);
			free(line);
			while (i > 0)
			{
				line = free_join("/tmp/heredoc_", ft_itoa(i--), 0 , 1);
				unlink(line);
				free(line);
			}
			return (ft_lstclear(ft_envp(NULL), free), \
					fd_printf(STDOUT_FILENO, "exit\n"), g_signal);
		}
		if (line)
			add_history(line);
		if (ft_strlen(line) == 0)
			continue ;
		handle_input(line);
	}
	// free(prompt);
	ft_lstclear(ft_envp(NULL), free);
	return (g_signal);
}
