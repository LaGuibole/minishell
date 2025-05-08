/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 12:16:50 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/08 20:28:33 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Frees environment and command list memory before exiting a child
/// @param cmd The command list to free
/// @param status The exit status code to return from the child process
void	exit_child(t_cmd *cmd, int status)
{
	ft_lstclear(ft_envp(NULL), free);
	free_cmd_list(cmd);
	exit(status);
}
