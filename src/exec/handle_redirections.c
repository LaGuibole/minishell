/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 17:53:51 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/29 15:31:41 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Apply input, output and heredoc redirections for a command
/// @param redir The redirection list to apply
void	apply_shell_redirections(t_redir *redir)
{
	int	last_input_fd;

	last_input_fd = -1;
	while (redir)
	{
		if (redir->type == R_INPUT || redir->type == R_HEREDOC)
			handle_input_types(redir, &last_input_fd);
		else if (redir->type == R_OUTPUT)
			redir_output(redir->filename);
		else if (redir->type == R_APPEND)
			redir_append(redir->filename);
		redir = redir->next;
	}
	if (last_input_fd != -1)
	{
		if (dup2(last_input_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 input");
			exit(EXIT_FAILURE);
		}
		close(last_input_fd);
	}
}

/// @brief Check if there is an output or append redirection inthe list
/// @param redir The redirection list to check
/// @return true if an output or append redirection is found, false otherwise
bool	has_output_redirections(t_redir *redir)
{
	while (redir)
	{
		if (redir->type == R_OUTPUT || redir->type == R_APPEND)
			return (true);
		redir = redir->next;
	}
	return (false);
}
