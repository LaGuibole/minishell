/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:35:22 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/29 13:41:29 by guphilip         ###   ########.fr       */
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

	signal(SIGINT, SIG_IGN);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, (char *)delimiter) == 0)
			break ;
		fd_printf(fd, "%s\n", line);
		free(line);
	}
	if (line)
		free(line);
	signal(SIGINT, SIG_DFL);
	return (RET_OK);
}

/// @brief Create a temporary file, write heredoc content to it and return it
///			as input
/// @param delimiter The delimiter that ends the heredoc input
/// @return A file descriptor opened in read-only mode, or -1 on error
char	*create_heredoc_fd(const char *delimiter)
{
	char	*path;
	int		fd;

	path = ft_mktemp();
	if (!path)
		return (NULL);
	fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		return (free(path), perror("heredoc: open"), NULL);
	if (write_heredoc_content(fd, delimiter) == -1)
		return (close(fd), unlink(path), free(path), NULL);
	close(fd);
	// fd_read = open(path, O_RDONLY);
	// if (fd_read == -1)
	// 	return (unlink(path), perror("heredoc: open"), free(path), NULL);
	// unlink(path);
	// close (fd_read);
	return (path);
}

void	prepare_heredocs(t_cmd *cmds)
{
	t_cmd	*cmd;
	t_redir	*redir;
	int		fd;
	char	*path;

	fd = -1;
	cmd = cmds;
	while (cmd)
	{
		redir = cmd->redir;
		while (redir)
		{
			if (redir->type == R_HEREDOC)
			{
				path = create_heredoc_fd(redir->filename);
				if (!path)
					break ;
				free(redir->filename);
				redir->filename = path;
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
}

void	close_other_heredocs(t_cmd *all_cmds, t_cmd *current)
{
	t_cmd	*tmp;
	t_redir	*redir;

	tmp = all_cmds;
	while (tmp)
	{
		if (tmp != current)
		{
			redir = tmp->redir;
			while (redir)
			{
				if (redir->type == R_HEREDOC && (tmp != current) && redir->fd != -1)
				{
					close(redir->fd);
					redir->fd = -1;
				}
				redir = redir->next;
			}
		}
		tmp = tmp->next;
	}
}

