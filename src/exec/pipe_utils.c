/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:13:11 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/05 14:58:52 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Redirect a given input fd to STDIN
/// @param input_fd The input file descriptor to redirect
void	redirect_input_fd(int input_fd, int *pipefd)
{
	(void) pipefd;
	if (input_fd != STDIN_FILENO)
	{
		if (dup2(input_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 input_fd failed");
			exit(EXIT_FAILURE);
		}
		close(input_fd);
	}
}

/// @brief Redirect STDOUT to the pipe if needed, or close the pipe otherwise
/// @param cmd The current command (unused)
/// @param pipefd The pipe file descriptor
/// @param has_next True if there is a next command in the pipeline
/// @param has_out_redir True if the command already has an output redirection
void	redirect_output_fd(
	t_cmd *cmd,
	int *pipefd,
	bool has_next,
	bool has_out_redir
)
{
	(void)cmd;
	if (has_next && pipefd[1] != -1)
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
	}
	if (pipefd && pipefd[1] != -1)
		close(pipefd[1]);
}
