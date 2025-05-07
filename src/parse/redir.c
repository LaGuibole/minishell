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

int	ft_skip_char(char *str, int i, int type)
{
	int	tmp;

	tmp = i;
	if (type == 1)
		while (str[i] && str[i] == ' ')
			i++;
	if (type == 2)
		while (str[i] && str[i] != ' ' && str[i] != '<' && str[i] != '>')
			i++;
	return (i - tmp);
}

int	set_filename(char *str, t_redir_type type, int i, t_cmd *cmd)
{
	char			*filename;
	int				fname_start;

	fname_start = i;
	i += ft_skip_char(str, i, 2);
	filename = ft_substr(str, fname_start, i - fname_start);
	if (!filename)
		return (print_error(ERR_REDIR));
	if (add_redir(cmd, type, filename))
		return (free(filename), print_error(ERR_REDIR));
	return (RET_OK);
}
