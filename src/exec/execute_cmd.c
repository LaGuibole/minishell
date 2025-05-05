/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:47:39 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/05 15:17:01 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Execute a command in a child process (builtin or external)
/// @param cmd The command structure containing the command and its arguments
/// @param envp The environment variables used for execve
void	exec_child_process(t_cmd *cmd)
{
	char	*path;
	char	**my_env;

	my_env = env_to_array();
	if (!my_env)
		return ;
	if (cmd_is_builtin(cmd->cmd))
		free_all_and_exit(cmd, exec_builtin(cmd));
	path = get_cmd_path(cmd, my_env);
	// if (strcmp(path, "<<") == 0)
	// 	return ;
	if (!path)
	{
		ft_lstclear(ft_envp(NULL), free);
		fd_printf(STDERR_FILENO, "%s: Command not found\n", cmd->cmd);
		free_cmd_list(cmd);
		free_double_tab(my_env);
		exit (127);
	}
	if (execve(path, cmd->params, my_env) == -1 && ft_strcmp(path, "<<") != 0)
	{
		perror("execve");
		free(path);
		free_cmd_list(cmd);
		free_double_tab(my_env);
		exit(EXIT_FAILURE);
	}
}

/// @brief Execute a single command (builtin or external), forking if necessary
/// @param cmd The command structure containing the command and its arguments
/// @param envp The environement variables used for execve
/// @return 0 on success, or 1 on fork error
int	exec_cmd(t_cmd *cmd)
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
		if (cmd->redir)
			apply_shell_redirections(cmd->redir);
		if (cmd_is_builtin(cmd->cmd))
			free_all_and_exit(cmd, exec_builtin(cmd));
		exec_child_process(cmd);
	}
	waitpid(pid, &status, 0);
	return (RET_OK);
}
