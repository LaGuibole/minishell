/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:06:08 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/29 13:55:31 by guphilip         ###   ########.fr       */
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
	else if (has_next)
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
// pid_t	fork_child(t_cmd *cmd, int input_fd, int *pipefd, char **envp)
// {
// 	pid_t	pid;
// 	bool	has_next;
// 	bool	has_out_redir;

// 	has_next = (cmd->next != NULL);
// 	has_out_redir = has_output_redirections(cmd->redir);
// 	pid = fork();
// 	if (pid == -1)
// 		return (-1);
// 	if (pid == 0)
// 	{
// 		if (cmd->redir)
// 			apply_shell_redirections(cmd->redir); // redirections du here_doc a revoir
// 		if (!cmd->redir)
// 			setup_pipe_redirections(input_fd, pipefd, has_next);
// 		else if (has_next && !has_out_redir)
// 		{
// 			close(pipefd[0]);
// 			dup2(pipefd[1], STDOUT_FILENO);
// 			close(pipefd[1]);
// 		}
// 		// close_other_heredocs(cmd->all_cmds, cmd);
// 		if (cmd->is_builtin)
// 			exit(exec_builtin(cmd));
// 		if (cmd->cmd)
// 			exec_child_process(cmd, envp);
// 		exit(EXIT_FAILURE);
// 	}
// 	return (pid);
// }

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
	bool	has_out_redir;

	has_next = (cmd->next != NULL);
	has_out_redir = has_output_redirections(cmd->redir);

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		if (input_fd != STDIN_FILENO)
		{
			if (dup2(input_fd, STDIN_FILENO) == -1)
			{
				perror("dup2 input_fd");
				exit(EXIT_FAILURE);
			}
			close(input_fd);
		}
		if (cmd->redir)
			apply_shell_redirections(cmd->redir);
		// close_other_heredocs(cmd->all_cmds, cmd);
		if (has_next)
		{
			close(pipefd[0]);
			if (!has_out_redir)
			{
				if (dup2(pipefd[1], STDOUT_FILENO) == -1)
				{
					perror("dup2 pipefd[1]");
					exit(EXIT_FAILURE);
				}
			}
			close(pipefd[1]);
		}
		if (cmd->is_builtin)
			exit(exec_builtin(cmd));
		if (cmd->cmd)
			exec_child_process(cmd, envp);
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
	else
	{
		close(pipefd[0]);
	}
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
		if (waitpid(cmd->pid, &last_status, 0) == -1)
			perror("waitpid error");
		if (WIFEXITED(last_status))
			g_signal = WEXITSTATUS(last_status);
		if (WIFSIGNALED(last_status))
			g_signal = 130;
		cmd = cmd->next;
	}
}

/// @brief Execute a pipeline of commands, handling pipes and process creation
/// @param cmds The linked list of commands forming the pipeline
/// @param envp The environment variables passed to execve
/// @return 0 on success, 1 on pipe or fork failure
int	exec_pipeline(t_cmd *cmds, char **envp)
{
	t_cmd	*curr;
	int		pipefd[2];
	int		input_fd;


	if (!cmds)
		return (RET_ERR);

	prepare_heredocs(cmds);
	if (!cmds->next)
		return (exec_cmd(cmds, envp));
	input_fd = STDIN_FILENO;
	curr = cmds;
	while (curr)
	{
		if (curr->next && pipe(pipefd) == -1)
			return (perror("pipe error"), 1);
		curr->pid = fork_child(curr, input_fd, pipefd, envp);
		if (curr->pid == -1)
			return (perror("fork error"), 1);
		input_fd = parent_cleanup(input_fd, pipefd, curr->next != NULL);
		curr = curr->next;
	}
	wait_children(cmds);
	return (RET_OK);
}
