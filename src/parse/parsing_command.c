/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlintot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 22:15:45 by mlintot           #+#    #+#             */
/*   Updated: 2025/04/14 22:15:46 by mlintot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_cmd(char *str, t_cmd *cmd)
{
	(void) str;
	(void) cmd;
	return (RET_ERR);
}

static int	set_parameters(char *str, t_cmd *cmd)
{
	(void) str;
	(void) cmd;
	return (RET_ERR);
}

static int	set_redirect(char *str, t_cmd *cmd)
{
	(void) str;
	(void) cmd;
	return (RET_ERR);
}

int	parse_cmd(char *str, t_cmd *cmd)
{
	if (
		set_cmd(str, cmd) || //Recuperation command
		set_parameters(str, cmd) || //Recuperation des parametres de la command
		set_redirect(str, cmd) //Recuperation des redirections de la command
	)
		return (RET_ERR);
	return (RET_OK);
}
