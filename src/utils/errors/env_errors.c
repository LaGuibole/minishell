/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_errors.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:16:33 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/16 19:48:52 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Print error message for invalid identifier
/// @param arg Invalid arg
void	print_invalid_identifier(char *arg)
{
	fd_printf(STDERR_FILENO, "minishell: export: `%s':\
		not a valid identifier\n", arg);
}
