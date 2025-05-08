/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:51:19 by mlintot           #+#    #+#             */
/*   Updated: 2025/05/08 20:52:57 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"

/// @brief Returns the last node of a command linked list
/// @param cmd The head of the command list
/// @return A pointer to the last t_cmd node, or NULL if the list is empty
t_cmd	*cmdlast(t_cmd *cmd)
{
	t_cmd	*temp;

	if (!cmd)
		return (NULL);
	temp = cmd;
	while (temp->next)
		temp = temp->next;
	return (temp);
}

/// @brief Appends a new command node to the end of the comman dlist
/// @param cmd A pointer to the head of the command list
/// @param new The new command node to add
void	cmdadd_back(t_cmd **cmd, t_cmd *new)
{
	t_cmd	*last;

	last = cmdlast(*cmd);
	if (!last)
		*cmd = new;
	else
		last->next = new;
}

/// @brief Allocates and init a new cmd struct with default values
/// @return A pointer to the newly allocated t_cmd struct, or NULL
t_cmd	*cmdnew(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd) + 1);
	if (!cmd)
		return (NULL);
	cmd->cmd = NULL;
	cmd->is_builtin = 0;
	cmd->pid = -1;
	cmd->params = NULL;
	cmd->nbparams = 0;
	cmd->redir = NULL;
	cmd->type_link_next = L_NONE;
	cmd->next = NULL;
	return (cmd);
}

/// @brief Sets the builtin flag of a command based on its name
/// @param cmd The command to analyze
/// @return RET_OK after setting the flag
int	set_is_builtin(t_cmd *cmd)
{
	cmd->is_builtin = cmd_is_builtin(cmd->cmd);
	return (RET_OK);
}

/// @brief Set the cmd field of a command to its first parameter if available
/// @param cmd The cmd to update
/// @return RET_OK after setting the field
int	set_cmd(t_cmd *cmd)
{
	if (cmd->params[0])
		cmd->cmd = cmd->params[0];
	return (RET_OK);
}
