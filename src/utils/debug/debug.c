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

static void	print_params(char **params)
{
	int	i;

	if (!params)
	{
		printf("  params: (null)\n");
		return ;
	}
	i = 0;
	while (params[i])
	{
		printf("  param[%d]: %s\n", i, params[i]);
		i++;
	}
}

static void	print_redir(t_redir *redir)
{
	while (redir)
	{
		printf("  redir: ");
		if (redir->type == R_INPUT)
			printf("< ");
		else if (redir->type == R_OUTPUT)
			printf("> ");
		else if (redir->type == R_APPEND)
			printf(">> ");
		else if (redir->type == R_HEREDOC)
			printf("<< ");
		printf("%s\n", redir->filename);
		redir = redir->next;
	}
}

static void	print_next_cmd(t_cmd *next)
{
	if (!next)
	{
		printf("  next: (null)\n");
		return ;
	}
	printf("  next: %s\n", next->cmd);
}

void	print_cmd_list(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd)
	{
		printf("=== Command %d ===\n", i);
		printf("  cmd: %s\n", cmd->cmd);
		printf("  is_builtin: %s\n", cmd->is_builtin ? "true" : "false");
		printf("  nbparams: %d\n", cmd->nbparams);
		print_params(cmd->params);
		print_redir(cmd->redir);
		if (cmd->type_link_next == L_PIPE)
			printf("  type_link_next: PIPE\n");
		else
			printf("  type_link_next: END\n");
		print_next_cmd(cmd->next);
		cmd = cmd->next;
		i++;
	}
}
