/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 21:23:59 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/23 21:24:26 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_params(char **params, int nbparams)
{
	int	i;

	if (!params)
	{
		ft_printf("  params: (null)\n");
		return ;
	}
	i = 0;
	while (i <= nbparams)
	{
		ft_printf("  param[%d]: %s\n", i, params[i]);
		i++;
	}
}

static void	print_redir(t_redir *redir)
{
	while (redir)
	{
		ft_printf("  redir: ");
		if (redir->type == R_INPUT)
			ft_printf("< ");
		else if (redir->type == R_OUTPUT)
			ft_printf("> ");
		else if (redir->type == R_APPEND)
			ft_printf(">> ");
		else if (redir->type == R_HEREDOC)
			ft_printf("<< ");
		ft_printf("%s\n", redir->filename);
		redir = redir->next;
	}
}

static void	print_next_cmd(t_cmd *next)
{
	if (!next)
	{
		ft_printf("  next: (null)\n");
		return ;
	}
	ft_printf("  next: %s\n", next->cmd);
}

void	print_cmd_list(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd)
	{
		ft_printf("=== Command %d ===\n", i);
		ft_printf("  cmd: %s\n", cmd->cmd);
		ft_printf("  is_builtin: %d\n", cmd->is_builtin);
		ft_printf("  nbparams: %d\n", cmd->nbparams);
		print_params(cmd->params, cmd->nbparams);
		print_redir(cmd->redir);
		if (cmd->type_link_next == L_PIPE)
			ft_printf("  type_link_next: PIPE\n");
		else
			ft_printf("  type_link_next: END\n");
		print_next_cmd(cmd->next);
		cmd = cmd->next;
		i++;
	}
}
