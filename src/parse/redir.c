/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlintot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 23:43:34 by mlintot           #+#    #+#             */
/*   Updated: 2025/04/22 23:43:35 by mlintot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	add_redir(t_cmd *cmd, t_redir_type type, char *filename)
{
	t_redir	*new;
	t_redir	*tmp;

	new = malloc(sizeof(t_redir));
	if (!new)
		return (RET_ERR);
	new->type = type;
	new->filename = filename;
	new->next = NULL;
	if (!cmd->redir)
		cmd->redir = new;
	else
	{
		tmp = cmd->redir;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (RET_OK);
}

bool	is_redir_char(char c)
{
	return (c == '<' || c == '>');
}
