/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:48:52 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/05 14:47:07 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	setup_pipe(int pipefd[2], bool has_next)
{
	if (has_next)
	{
		if (pipe(pipefd) == -1)
			return(perror("pipe error"), RET_ERR);
	}
	else
	{
		pipefd[0] = -1;
		pipefd[1] = -1;
	}
	return (RET_OK);
}
