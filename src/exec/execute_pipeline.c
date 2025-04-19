/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:06:08 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/19 18:16:57 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Set up stdin and stdout redirections for child procees
/// @param input_fd The fd to use as STDIN
/// @param pipefd The pipe array (only used if the command is not last)
/// @param has_next True if there is a next command
void	setup_pipe_redirections(int input_fd, int *pipefd, bool has_next)
{
	if (input_fd != STDIN_FILENO)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	if (has_next)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
}

/// @brief Forks and executes a command in a pipeline
/// @param cmd Current command
/// @param input_fd fd for input (from previous pipe)
/// @param pipefd Current pipe (to next process)
/// @param envp Environement variables
/// @return pid of the child processm or -1 on error
pid_t	fork_child(t_cmd *cmd, int input_fd, int *pipefd, char **envp)
{
	pid_t	pid;
	bool	has_next;

	has_next = (cmd->next != NULL);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		setup_pipe_redirections(input_fd, pipefd, has_next);
		if (cmd->redir)
			apply_shell_redirections(cmd->redir);
		if (cmd_is_builtin(cmd->cmd))
			exit(exec_builtin(cmd));
		exec_cmd(cmd, envp);
		exit(EXIT_FAILURE);
	}
	return (pid);
}

/// @brief Cleans up unused pipe ends and returns next input fd
/// @param input_fd Previous input fd
/// @param pipefd Current pipe array
/// @param has_next True if there is a next command
/// @return Next input fd (pipefd[0]) or STDIN if last
int	parent_cleanup(int input_fd, int *pipefd, bool has_next)
{
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	if (has_next)
	{
		close(pipefd[1]);
		return (pipefd[0]);
	}
	return (STDIN_FILENO);
}

/// @brief Waits for all children processes to finish
void	wait_children(void)
{
	while (wait(NULL) > 0)
		;
}

int	exec_pipeline(t_cmd *cmds, char **envp)
{
	t_cmd	*curr;
	int		pipefd[2];
	int		input_fd;
	pid_t	pid;

	input_fd = STDIN_FILENO;
	curr = cmds;
	while (curr)
	{
		if (curr->next && pipe(pipefd) == -1)
			return (perror("pipe error"), 1);
		pid = fork_child(curr, input_fd, pipefd, envp);
		if (pid == -1)
			return (perror("fork error"), 1);
		input_fd = parent_cleanup(input_fd, pipefd, curr->next != NULL);
		curr = curr->next;
	}
	wait_children();
	return (0);
}
