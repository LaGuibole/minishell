/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_process.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 11:03:21 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/06 18:03:35 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Forks and executes a command in a pipeline
/// @param cmd Current command
/// @param input_fd fd for input (from previous pipe)
/// @param pipefd Current pipe (to next process)
/// @param envp Environement variables
/// @return pid of the child processm or -1 on error
pid_t	fork_child(t_exec_ctx *ctx, int input_fd, int *pipefd)
{
	pid_t	pid;
	bool	has_next;
	bool	has_out_redir;

	has_next = (ctx->curr->next != NULL);
	has_out_redir = has_output_redirections(ctx->curr->redir);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		redirect_input_fd(input_fd, pipefd);
		if (ctx->curr->redir)
			apply_shell_redirections(ctx->curr->redir);
		redirect_output_fd(ctx->curr, pipefd, has_next, has_out_redir);
		if (ctx->curr->is_builtin)
			exit_child(ctx->curr, exec_builtin(ctx));
		if (ctx->curr->cmd)
			exec_child_process(ctx);
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
	if (pipefd)
	{
		if (has_next && pipefd[1] != -1)
			close(pipefd[1]);
		if (!has_next && pipefd[0] != -1)
			close(pipefd[0]);
	}
	if (has_next && pipefd && pipefd[0] != -1)
		return (pipefd[0]);
	return (STDIN_FILENO);
}

/// @brief Waits for all children processes to finish
void	wait_children(t_cmd *cmds)
{
	t_cmd	*cmd;
	int		last_status;

	cmd = cmds;
	while (cmd)
	{
		if (cmd->pid != -2)
		{
			if (waitpid(cmd->pid, &last_status, 0) == -1)
				perror("waitpid error");
			if (WIFEXITED(last_status))
				g_signal = WEXITSTATUS(last_status);
			if (WIFSIGNALED(last_status))
				g_signal = 130;
		}
		cmd = cmd->next;
	}
}
