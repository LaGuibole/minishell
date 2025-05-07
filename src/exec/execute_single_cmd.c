/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_single_cmd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:47:39 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/06 18:02:35 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"

static void	execve_cmd(char *path, t_exec_ctx *ctx, char **my_env);

/// @brief Execute a single command (builtin or external), forking if necessary
/// @param cmd The command structure containing the command and its arguments
/// @param envp The environement variables used for execve
/// @return 0 on success, or 1 on fork error
int	exec_cmd(t_exec_ctx *ctx)
{
	pid_t	pid;
	int		status;

	if (cmd_is_builtin(ctx->head->cmd) && !is_builtin_pipeable(ctx->head->cmd))
		return (exec_builtin(ctx));
	pid = fork();
	if (pid == -1)
		return (perror("fork"), RET_ERR);
	if (pid == 0)
	{
		if (ctx->head->redir)
			apply_shell_redirections(ctx->head->redir);
		if (cmd_is_builtin(ctx->head->cmd))
			free_all_and_exit(ctx->head, exec_builtin(ctx));
		exec_child_process(ctx);
	}
	waitpid(pid, &status, 0);
	return (RET_OK);
}

/// @brief Execute a command in a child process (builtin or external)
/// @param cmd The command structure containing the command and its arguments
/// @param envp The environment variables used for execve
void	exec_child_process(t_exec_ctx *ctx)
{
	char	*path;
	char	**my_env;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	my_env = env_to_array();
	if (!my_env)
		return ;
	path = get_cmd_path(ctx->curr, my_env);
	if (!path)
	{
		ft_lstclear(ft_envp(NULL), free);
		fd_printf(STDERR_FILENO, "%s: Command not found\n", ctx->curr->cmd);
		free_cmd_list(ctx->head);
		free_double_tab(my_env);
		exit (127);
	}
	execve_cmd(path, ctx, my_env);
}

static void	execve_cmd(char *path, t_exec_ctx *ctx, char **my_env)
{
	if (execve(path, ctx->curr->params, my_env) == -1
		&& ft_strcmp(path, "<<") != 0)
	{
		perror("execve");
		free(path);
		free_cmd_list(ctx->head);
		free_double_tab(my_env);
		exit(EXIT_FAILURE);
	}
}
