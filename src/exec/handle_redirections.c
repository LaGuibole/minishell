/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 17:53:51 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/05 14:40:28 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_redir_file(t_redir *redir)
{
	int	fd;

	fd = -1;
	if (redir->type == R_INPUT || redir->type == R_HEREDOC)
		fd = open(redir->filename, O_RDONLY);
	else if (redir->type == R_OUTPUT)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == R_APPEND)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(redir->filename);
		return (RET_ERR);
	}
	close(fd);
	return (RET_OK);
}

int	validate_redirections(t_cmd *cmds)
{
	t_cmd	*curr;
	t_redir	*redir;

	curr = cmds;
	while (curr)
	{
		redir = curr->redir;
		while (redir)
		{
			if (check_redir_file(redir) != RET_OK)
				return (RET_ERR);
			redir = redir->next;
		}
		curr = curr->next;
	}
	return (RET_OK);
}

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
