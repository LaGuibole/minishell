/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 18:41:48 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/08 21:24:54 by guphilip         ###   ########.fr       */
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
