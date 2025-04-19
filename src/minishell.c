/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:04:50 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/19 16:51:15 by guphilip         ###   ########.fr       */
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

// int	main(int argc, char **argv, char **envp)
// {
// 	struct sigaction	sa_c;

// 	(void) argc;
// 	(void) argv;
// 	int heredoc_fd = -1;
// 	sa_c.sa_handler = handle_sigint;
// 	sa_c.sa_flags = 0;
// 	sigemptyset(&sa_c.sa_mask);
// 	sigaction(SIGINT, &sa_c, NULL);
// 	signal(SIGQUIT, SIG_IGN);
// 	ft_envp(envp);
// 	while (1)
// 	{
// 		char *prompt = display_prompt();
// 		char *line = readline(prompt);
// 		if (!line)
// 			return (free(prompt), ft_lstclear(ft_envp(NULL), free), \
// 					ft_printf("exit\n"), 1);
// 		if (*line)
// 			add_history(line);
// 		char **args = split_free(line, ' ', false);
// 		if (args && args[0] && ft_strncmp(args[0], "heredoc", 8) == 0)
// 			heredoc_fd = create_heredoc_fd("EOF");
// 		exec_builtin(args);
// 		free_double_tab(args);
// 		free(prompt);
// 		free(line);
// 		if (heredoc_fd != -1)
// 			close(heredoc_fd);
// 	}
// 	ft_lstclear(ft_envp(NULL), free);
// 	return (RET_OK);
// }

// int	main(int argc, char **argv, char **envp)
// {
// 	struct sigaction	sa_c;

// 	(void) argc;
// 	(void) argv;
// 	// int heredoc_fd = -1;
// 	sa_c.sa_handler = handle_sigint;
// 	sa_c.sa_flags = 0;
// 	sigemptyset(&sa_c.sa_mask);
// 	sigaction(SIGINT, &sa_c, NULL);
// 	signal(SIGQUIT, SIG_IGN);
// 	ft_envp(envp);
// 	while (1)
// 	{
// 		char	*prompt = display_prompt();
// 		char	*line = readline(prompt);
// 		if (!line)
// 		{
// 			free(prompt);
// 			ft_lstclear(ft_envp(NULL), free);
// 			fd_printf(STDOUT_FILENO, "exit\n");
// 			return (1);
// 		}
// 		if (*line)
// 			add_history(line);
// 		char **args = split_free(line, ' ', false);
// 		if (args && args[0])
// 		{
// 			t_cmd cmd;

// 			cmd.cmd = args[0];
// 			cmd.next = args[1];
// 			cmd.params = args;
// 			cmd.redir = NULL;
// 			cmd.type_link_next = L_BACK;
// 			exec_cmd(&cmd, envp);
// 		}
// 		free_double_tab(args);
// 		free(prompt);
// 		free(line);
// 	}
// }

static void	free_cmd_chain(t_cmd *cmd)
{
	t_cmd	*next;
	size_t	i;

	while (cmd)
	{
		if (cmd->params)
		{
			i = 0;
			while (cmd->params[i])
			{
				free(cmd->params[i]);
				i++;
			}
			free(cmd->params);
		}
		// tu pourrais aussi free(cmd->cmd) si jamais il est ft_strdup
		next = cmd->next;
		// si redir est alloc, tu pourrais free ici aussi
		cmd = next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_cmd	cmd1;
	t_cmd	cmd2;
	t_cmd	cmd3;

	(void)argc;
	(void)argv;

	// Commande 1 : ls -l
	cmd1.cmd = "ls";
	cmd1.params = ft_split("ls -l", ' ');
	cmd1.redir = NULL;
	cmd1.type_link_next = L_PIPE;
	cmd1.next = &cmd2;

	// Commande 2 : grep minishell
	cmd2.cmd = "grep";
	cmd2.params = ft_split("grep minishell", ' ');
	cmd2.redir = NULL;
	cmd2.type_link_next = L_PIPE;
	cmd2.next = &cmd3;

	// Commande 3 : wc -l
	cmd3.cmd = "wc";
	cmd3.params = ft_split("wc -l", ' ');
	cmd3.redir = NULL;
	cmd3.type_link_next = L_BACK;
	cmd3.next = NULL;

	// Exécution pipeline (3 commandes)
	exec_pipeline(&cmd1, envp);

	// // Builtin simple : echo hello
	// t_cmd	builtin;
	// builtin.cmd = "echo";
	// builtin.params = ft_split("echo hello", ' ');
	// builtin.redir = NULL;
	// builtin.type_link_next = L_BACK;
	// builtin.next = NULL;
	// exec_cmd(&builtin, envp);


	// int fd = create_heredoc_fd("EOF");
	// write(fd, "test\n42\nEOF\n", 12);
	// close(fd);

	// Nettoyage
	free_cmd_chain(&cmd1);
	// free_double_tab(builtin.params);

	return (0);
}

