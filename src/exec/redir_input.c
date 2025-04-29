/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:13:22 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/29 15:38:11 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Open a file for input redirection
/// @param filename The name of the file to open
/// @return The file descriptor opened in read-only mode
int	handle_input_redirection(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("open input");
		exit(EXIT_FAILURE);
	}
	return (fd);
}

/// @brief Open a temporary heredoc file for input redirection
/// @param filename The name of the heredoc temporary file
/// @return The file descriptor opened in read-only mode
int	handle_heredoc_redirection(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("open heredoc");
		exit(EXIT_FAILURE);
	}
	return (fd);
}

/// @brief Handle input or heredoc redirection for a command
/// @param redir The current redirection mode
/// @param last_input_fd Pointer to the last input file descriptor to update
void	handle_input_types(t_redir *redir, int *last_input_fd)
{
	if (*last_input_fd != -1)
		close(*last_input_fd);
	if (redir->type == R_INPUT)
		*last_input_fd = handle_input_redirection(redir->filename);
	else if (redir->type == R_HEREDOC)
		*last_input_fd = handle_heredoc_redirection(redir->filename);
}
