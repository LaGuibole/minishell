/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:47:39 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/18 17:09:09 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_cmd(t_cmd *cmd, char **envp)
{
	char	*path;

	cmd->is_builtin = cmd_is_builtin(cmd->cmd);
	if (cmd->is_builtin)
		return (exec_builtin(cmd));
	path = get_cmd_path(cmd, envp);
	if (!path)
	{
		fd_printf(STDERR_FILENO, "Command not found\n");
		return (127);
	}
	int pid = fork();
	if (pid != 0)
		return (waitpid(-1, NULL, 0), RET_OK);
	if (execve(path, cmd->params, envp) == -1)
	{
		perror("execve");
		free(path);
		return (RET_ERR);
	}
	return (RET_OK);
}
