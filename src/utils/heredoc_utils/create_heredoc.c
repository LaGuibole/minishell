/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:35:22 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/18 13:01:30 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
