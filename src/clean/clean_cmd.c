/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:51:07 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/08 20:27:35 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Frees a two-dimensional array of strings up to a specified num of el
/// @param str The array of strings to free
/// @param nbr The number of strings to free before freeing the array itself
void	free_strstr(char **str, int nbr)
{
	int	i;

	i = 0;
	if (str)
	{
		while (i < nbr)
			free(str[i++]);
		free(str);
	}
}

/// @brief Recursively frees a linked list of redirection structures
/// @param redir The head of the redir list to free
void	free_redir(t_redir *redir)
{
	if (redir)
	{
		if (redir->next)
			free_redir(redir->next);
		if (redir->filename)
			free(redir->filename);
		free(redir);
	}
}

/// @brief Free all memory associated with a single command structure,
///			including parameters and redirections
/// @param cmd The command structure to free
void	free_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (cmd->nbparams > 0)
	{
		while (i < cmd->nbparams)
			free(cmd->params[i++]);
	}
	if (cmd->params)
		free(cmd->params);
	free_redir(cmd->redir);
	free(cmd);
}

/// @brief Recursively frees a linked list of command structures
/// @param cmd The head of the command list to free
void	free_cmd_list(t_cmd *cmd)
{
	if (cmd)
	{
		if (cmd->next)
			free_cmd_list(cmd->next);
		free_cmd(cmd);
	}
}
