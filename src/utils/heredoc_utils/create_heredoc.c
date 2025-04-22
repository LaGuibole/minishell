/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:35:22 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/22 17:31:03 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Read lines from user input until the delimiter is matched and
///			write then to a file
/// @param fd The file descriptor to write the heredoc content to
/// @param delimiter The string that terminates the heredoc input
/// @return (RET_OK) -- > 0 on completion
static int	write_heredoc_content(int fd, const char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, (char *)delimiter) == 0)
			break ;
		fd_printf(fd, "%s\n", line);
		free(line);
	}
	free(line);
	return (RET_OK);
}

/// @brief Create a temporary file, write heredoc content to it and return it
///			as input
/// @param delimiter The delimiter that ends the heredoc input
/// @return A file descriptor opened in read-only mode, or -1 on error
int	create_heredoc_fd(const char *delimiter)
{
	char	*path;
	int		fd;

	path = ft_mktemp();
	if (!path)
		return (-1);
	fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		return (free(path), perror("heredoc: open"), -1);
	write_heredoc_content(fd, delimiter);
	close(fd);
	fd = open(path, O_RDONLY);
	unlink(path);
	free(path);
	return (fd);
}
