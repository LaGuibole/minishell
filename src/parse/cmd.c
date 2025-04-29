/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:51:19 by mlintot           #+#    #+#             */
/*   Updated: 2025/04/29 13:46:51 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	cmdadd_back(t_cmd **cmd, t_cmd *new)
{
	t_cmd	*last;

	last = cmdlast(*cmd);
	if (!last)
		*cmd = new;
	else
		last->next = new;
}

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
