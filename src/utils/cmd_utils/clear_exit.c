/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:34:03 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/22 16:02:55 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redirections(t_redir *redir_list)
{
	t_redir	*tmp;

	while (redir_list)
	{
		tmp = redir_list->next;
		if (redir_list)
		{
			free(redir_list->filename);
		}
		free(redir_list);
		redir_list = tmp;
	}
}

void	free_all_and_exit(t_cmd *cmd, int code)
{
	free_double_tab(cmd->params);
	if (cmd->redir)
		free_redirections(cmd->redir);
	ft_lstclear(ft_envp(NULL), free);
	rl_clear_history();
	exit(code);
}
