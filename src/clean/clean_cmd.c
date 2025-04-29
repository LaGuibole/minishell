/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:51:07 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/25 11:25:34 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*void	free_cmd_list(t_cmd *cmd)
{
	t_cmd	*next;

	while (cmd)
	{
		next = cmd->next;
		if (cmd->cmd)
			free(cmd->cmd);
		free_double_tab(cmd->params);
		free_redirections(cmd->redir);
		free(cmd);
		cmd = next;
	}
}*/

void free_redir(t_redir *redir)
{
	if (redir)
	{
		if (redir->next)
			free_redir(redir->next);
		if (redir->filename)
			free(redir->filename);
		free(redir);
	}
}

void free_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (cmd->nbparams > 0)
		while (i < cmd->nbparams)
			free(cmd->params[i++]);
	free_redir(cmd->redir);
	free(cmd);
}

void	free_cmd_list(t_cmd *cmd)
{
	if (cmd)
	{
		if (cmd->next)
			free_cmd_list(cmd->next);
		free_cmd(cmd);
	}
}