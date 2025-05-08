/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:48:52 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/08 20:36:33 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Skips over an empty command in a pipeline (i.e, one without an exec
///			name). Close the current input FD if needed and sets the command's
///			PID to -2
/// @param curr A pointer to the current pointer (will be advanced to the next)
/// @param input_fd A pointer to the current input_fd |reset to STDIN if closed
/// @return true if the command was empty and skipped, false otherwise
bool	skip_empty_cmd(t_cmd **curr, int *input_fd)
{
	if (!(*curr)->cmd)
	{
		if (*input_fd != STDIN_FILENO)
			close(*input_fd);
		*input_fd = STDIN_FILENO;
		(*curr)->pid = -2;
		*curr = (*curr)->next;
		return (true);
	}
	return (false);
}

/// @brief Sets up a pipe if another command follows in the pipeline
///			If not, fills the pipe fd with -1
/// @param pipefd An array to store the read and write ends of the pipe
/// @param has_next Whether there is another command after the current one
/// @return RET_OK on success, RET_ERR on pipe creation failure
int	setup_pipe(int pipefd[2], bool has_next)
{
	if (has_next)
	{
		if (pipe(pipefd) == -1)
			return (perror("pipe error"), RET_ERR);
	}
	else
	{
		pipefd[0] = -1;
		pipefd[1] = -1;
	}
	return (RET_OK);
}
