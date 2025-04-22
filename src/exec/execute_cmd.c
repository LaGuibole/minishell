/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:47:39 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/22 16:14:43 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_child_process(t_cmd *cmd, char **envp)
{
	char	*path;

	if (cmd_is_builtin(cmd->cmd))
		free_all_and_exit(cmd, exec_builtin(cmd));
	path = get_cmd_path(cmd, envp);
	if (!path)
	{
		fd_printf(STDERR_FILENO, "Command not found\n");
		exit (127);
	}
	if (execve(path, cmd->params, envp) == -1)
	{
		perror("execve");
		free(path);
		exit(EXIT_FAILURE);
	}
}

int	exec_cmd(t_cmd *cmd, char **envp)
{
	pid_t	pid;
	int		status;

	if (cmd_is_builtin(cmd->cmd) && !is_builtin_pipeable(cmd->cmd))
		return (exec_builtin(cmd));
	pid = fork();
	if (pid == -1)
		return (perror("fork"), RET_ERR);
	if (pid == 0)
	{
		if (cmd_is_builtin(cmd->cmd))
			free_all_and_exit(cmd, exec_builtin(cmd));
		exec_child_process(cmd, envp);
	}
	waitpid(pid, &status, 0);
	return (RET_OK);
}
