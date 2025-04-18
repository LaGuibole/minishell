/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dummy.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:41:28 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/18 16:42:22 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_dummy_params(t_cmd *cmd)
{
	if (!cmd || !cmd->cmd)
		return ;
	cmd->params = ft_split(cmd->cmd, ' ');
}
