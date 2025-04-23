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

t_redir	*rdrlast(t_redir *rdr)
{
	t_redir	*temp;

	if (!rdr)
		return (NULL);
	temp = rdr;
	while (temp->next)
		temp = temp->next;
	return (temp);
}

void	rdradd_back(t_redir **rdr, t_redir *new)
{
	t_redir	*last;

	last = rdrlast(*rdr);
	if (!last)
		*rdr = new;
	else
		last->next = new;
}

t_redir	*rdrnew(void)
{
	t_redir	*rdr;

	rdr = malloc(sizeof(t_redir) + 1);
	if (!rdr)
		return (NULL);
	rdr->type = R_NONE;
	rdr->filename = NULL;
	rdr->next = NULL;
	return (rdr);
}
