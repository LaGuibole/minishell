/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlintot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:47:16 by mlintot           #+#    #+#             */
/*   Updated: 2025/04/14 14:47:17 by mlintot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_next_cmd(char *str, t_cmd *cmd)
{
	(void) str;
	(void) cmd;
	return (RET_ERR);
}

static int	set_is_builtin(t_cmd *cmd)
{
	(void) cmd;
	return (RET_ERR);
}

static int	empty_link(char *str)
{
	(void) str;
	return (RET_ERR);
}

static int	unclosed_quotes(char *str)
{
	(void) str;
	return (RET_ERR);
}

t_cmd	*parsing_cmd(char *str)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd) + 1);
	if (
		unclosed_quotes(str) || //Gestion ' et " pour readline
		empty_link(str) || //Separation | et & pour la structure
		parse_cmd(str, cmd) || //Separation cmd, parameters et redirection
		set_is_builtin(cmd) || //Gestion is_builtin
		set_next_cmd(str, cmd) //Gestion next
	)
	{
		free(cmd);
		return (NULL);
	}
	return (cmd);
}
