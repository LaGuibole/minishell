/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 18:41:48 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/09 02:52:01 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Sets up signal handlers for SIGINT and SIGQUIT during command exec
///			Redirects both signals to a custom handler used in the execution
///			phase
void	set_sig_executing(void)
{
	signal(SIGINT, handle_signal_executing);
	signal(SIGQUIT, handle_signal_executing);
}

void	handle_sig_heredoc(int sig)
{
	(void)sig;
	g_signal = 1;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
	//ajouter les free des child pour le heredoc
}
