/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:34:03 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/25 21:17:46 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Free a linked list or redirections structures
/// @param redir_list The list of redirections to free
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

/// @brief Free all resources related to a command and exit the process
/// @param cmd The command structure to clean up
/// @param code The exit code to return from the process
void	free_all_and_exit(t_cmd *cmd, int code)
{
	free_double_tab(cmd->params);
	if (cmd->redir)
		free_redirections(cmd->redir);
	ft_lstclear(ft_envp(NULL), free);
	free(cmd);
	rl_clear_history();
	exit(code);
}
