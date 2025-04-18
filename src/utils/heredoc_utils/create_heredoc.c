/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:35:22 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/18 11:57:51 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_heredoc_fd(const char *delimiter)
{
	char	*line;
	char	*path;
	int		fd;

	path = ft_mktemp();
	if (!path)
		return (-1);
	fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("heredoc: open");
		free(path);
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, (char *)delimiter) == 0)
			break ;
		fd_printf(fd, "%s\n", line);
		free(line);
	}
	free(line);
	close(fd);
	fd = open(path, O_RDONLY);
	unlink(path);
	free(path);
	close(fd); // doute, est-ce que je le garde en lecture du coup ou est ce que je dois close en dehors de la fonction ?
	return (fd);
}
