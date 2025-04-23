/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:04:50 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/19 18:15:45 by guphilip         ###   ########.fr       */
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

int main(void)
{
	t_cmd *cmd;


	/*char *tmp[] = {"cat", "-nE", "Makefile", ">", "poubelle", NULL};
	execve("/usr/bin/cat", tmp, NULL);
	perror("execve");*/
	while (1)
	{
		char	*prompt = display_prompt();
		char	*line = readline(prompt);
	
		if (*line)
		{
			add_history(line);
			cmd = parsing_cmd(line);
			if (!cmd)
				ft_printf("\n");
			else
				free(cmd);
		}
		free(prompt);
		free(line);
	}
}

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

/*static void	free_cmd_chain(t_cmd *cmd)
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
}*/

// int	main(int argc, char **argv, char **envp)
// {
// 	t_cmd	cmd1;
// 	t_cmd	cmd2;
// 	t_cmd	cmd3;
// 	t_cmd	cmd4;
// 	t_cmd	cmd5;
// 	t_cmd	builtin;

// 	(void)argc;
// 	(void)argv;

// 	// Commande 1 : ls -l
// 	cmd1.cmd = "ls";
// 	cmd1.params = ft_split("ls -l", ' ');
// 	cmd1.redir = NULL;
// 	cmd1.type_link_next = L_PIPE;
// 	cmd1.next = &cmd2;

// 	// Commande 2 : grep minishell
// 	cmd2.cmd = "cat";
// 	cmd2.params = ft_split("cat Makefile", ' ');
// 	cmd2.redir = NULL;
// 	cmd2.type_link_next = L_PIPE;
// 	cmd2.next = &cmd3;

// 	// Commande 3 : wc -l
// 	cmd3.cmd = "grep";
// 	cmd3.params = ft_split("grep $", ' ');
// 	cmd3.redir = NULL;
// 	cmd3.type_link_next = L_PIPE;
// 	cmd3.next = &cmd4;

// 	// Commande 4 : wc -l
// 	cmd4.cmd = "wc";
// 	cmd4.params = ft_split("wc -l", ' ');
// 	cmd4.redir = NULL;
// 	cmd4.type_link_next = L_PIPE;
// 	cmd4.next = &builtin;

// 	//Commande 5 : builtin
// 	builtin.cmd = "echo";
// 	builtin.params = ft_split("echo -n coucou", ' ');
// 	builtin.redir = NULL;
// 	builtin.type_link_next = L_PIPE;
// 	builtin.next = &cmd5;

// 	cmd5.cmd = "heredoc";
// 	int fd = create_heredoc_fd("EOF");
// 	write(fd, "test\n42\nEOF\n", 12);
// 	close(fd);
// 	// Exécution pipeline (3 commandes)
// 	exec_pipeline(&cmd1, envp);


// 	// Nettoyage
// 	free_cmd_chain(&cmd1);
// 	free_double_tab(builtin.params);

// 	return (0);
// }
/*
int	main(int argc, char **argv, char **envp)
{
	t_cmd		cmd1;
	t_cmd		cmd2;
	t_cmd		cmd3;
	t_redir		*redir1;
	t_redir		*redir2;
	t_redir		*redir_in;
	t_list		*lst1;
	t_list		*lst2;

	(void)argc;
	(void)argv;

	// --- cmd1 : echo coucou > test1 > test2
	cmd1.cmd = "echo";
	cmd1.params = ft_split("echo coucou", ' ');
	cmd1.type_link_next = L_PIPE;
	cmd1.next = &cmd2;

	redir1 = malloc(sizeof(t_redir));
	redir1->type = R_OUTPUT;
	redir1->filename = ft_strdup("test1");

	redir2 = malloc(sizeof(t_redir));
	redir2->type = R_OUTPUT;
	redir2->filename = ft_strdup("test2");

	lst1 = ft_lstnew(redir1);
	lst2 = ft_lstnew(redir2);
	ft_lstadd_back(&lst1, lst2);

	cmd1.redir = lst1;

	// --- cmd2 : < test2 grep coucou
	cmd2.cmd = "grep";
	cmd2.params = ft_split("grep coucou", ' ');
	cmd2.type_link_next = L_PIPE;
	cmd2.next = &cmd3;

	redir_in = malloc(sizeof(t_redir));
	redir_in->type = R_INPUT;
	redir_in->filename = ft_strdup("test2");
	cmd2.redir = ft_lstnew(redir_in);

	// --- cmd3 : wc -l
	cmd3.cmd = "wc";
	cmd3.params = ft_split("wc -l", ' ');
	cmd3.type_link_next = L_BACK;
	cmd3.redir = NULL;
	cmd3.next = NULL;

	// --- Exécution
	exec_pipeline(&cmd1, envp);

	// --- Nettoyage
	free_cmd_chain(&cmd1);
	return (0);
}*/

