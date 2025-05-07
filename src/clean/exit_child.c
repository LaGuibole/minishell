/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 12:16:50 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/30 12:21:00 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_child(t_cmd *cmd, int status)
{
	ft_lstclear(ft_envp(NULL), free);
	free_cmd_list(cmd);
	exit(status);
}
