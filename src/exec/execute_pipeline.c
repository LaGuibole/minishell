/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:06:08 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/06 17:39:27 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	prepare_checks(t_cmd *cmds);
static int	try_setup_pipe(int *pipefd, int has_next);

/// @brief Execute a pipeline of commands, handling pipes and process creation
/// @param cmds The linked list of commands forming the pipeline
/// @param envp The environment variables passed to execve
/// @return 0 on success, 1 on pipe or fork failure
int	exec_pipeline(t_cmd *cmds)
{
	t_cmd	*curr;
	int		pipefd[2];
	int		input_fd;

	if (prepare_checks(cmds) != RET_OK)
		return (RET_ERR);
	if (!cmds->next && cmds->cmd)
		return (exec_cmd(cmds));
	input_fd = STDIN_FILENO;
	curr = cmds;
	while (curr)
	{
		if (skip_empty_cmd(&curr, &input_fd))
			continue ;
		if (try_setup_pipe(pipefd, curr->next != NULL) != RET_OK)
			return (RET_ERR);
		curr->pid = fork_child(curr, input_fd, pipefd);
		if (curr->pid == -1)
			return (perror("error"), RET_ERR);
		input_fd = parent_cleanup(input_fd, pipefd, curr->next != NULL);
		curr = curr->next;
	}
	wait_children(cmds);
	return (RET_OK);
}

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

/// @brief
/// @param cmds
/// @return
static int	prepare_checks(t_cmd *cmds)
{
	if (!cmds)
		return (RET_ERR);
	prepare_heredocs(cmds);
	set_sig_executing();
	if (validate_redirections(cmds) != RET_OK)
		return (RET_ERR);
	return (RET_OK);
}

/// @brief
/// @param pipefd
/// @param has_next
/// @return
static int	try_setup_pipe(int *pipefd, int has_next)
{
	int	result;

	result = setup_pipe(pipefd, has_next);
	if (result != RET_OK)
	{
		perror("error");
		return (RET_ERR);
	}
	return (RET_OK);
}
