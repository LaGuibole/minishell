/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:35:22 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/09 11:18:04 by guphilip         ###   ########.fr       */
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

	path = ft_mktemp(0);
	if (!path)
		return (NULL);
	fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		return (free(path), perror("heredoc: open"), NULL);
	if (write_heredoc_content(fd, delimiter) == -1)
		return (close(fd), unlink(path), free(path), NULL);
	close(fd);
	return (path);
}

/// @brief Prepares heredoc redirections for all commands in the pipeline
///			For each heredoc, creates a temporary file and replaces the redir
///			filename with the path to this temporary file
/// @param cmds The list of commands to process
void	prepare_heredocs(t_cmd *cmds)
{
	t_cmd	*cmd;
	t_redir	*redir;
	char	*path;

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

// int	write_heredoc_content(int fd, const char *delimiter)
// {
// 	char	*line;

// 	line = NULL;
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (line == NULL)
// 			break ;
// 		if (g_heredoc_sigint)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		if (ft_strcmp(line, delimiter) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		fd_printf(fd, "%s\n", line);
// 		free(line);
// 	}
// 	return (g_heredoc_sigint);
// }

// int	create_heredoc_fd(const char *delimiter, char **path_out)
// {
// 	char	*path;
// 	int		fd;
// 	pid_t	pid;
// 	int		status;

// 	path = ft_mktemp();
// 	if (!path)
// 		return (-1);
// 	fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
// 	if (fd == -1)
// 	{
// 		free(path);
// 		return (-1);
// 	}

// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		signal(SIGINT, handle_sigint_heredoc);
// 		signal(SIGQUIT, SIG_IGN);
// 		if (write_heredoc_content(fd, delimiter))
// 			exit(130);
// 		close(fd);
// 		exit(0);
// 	}
// 	close(fd);
// 	waitpid(pid, &status, 0);

// 	if (WIFSIGNALED(status) || WEXITSTATUS(status) == 130)
// 	{
// 		unlink(path);
// 		free(path);
// 		return (-1);
// 	}
// 	*path_out = path;
// 	return (0);
// }

// void	prepare_heredocs(t_cmd *cmds)
// {
// 	t_cmd	*cmd;
// 	t_redir	*redir;
// 	char	*path;

// 	cmd = cmds;
// 	while (cmd)
// 	{
// 		redir = cmd->redir;
// 		while (redir)
// 		{
// 			if (redir->type == R_HEREDOC)
// 			{
// 				path = NULL;
// 				if (create_heredoc_fd(redir->filename, &path) == -1)
// 					break ;
// 				free(redir->filename);
// 				redir->filename = path;
// 			}
// 			redir = redir->next;
// 		}
// 		cmd = cmd->next;
// 	}
// }
