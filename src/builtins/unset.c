/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 17:22:39 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/16 19:25:34 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Builtin implementation of the unset command
/// @param args The argument list passed to unset (args[0] is "unset")
/// @return 0 always (RET_OK)
int	ft_unset(char **args)
{
	int	i;

	if (!args || !args[1])
		return (RET_OK);
	i = 1;
	while (args[i])
	{
		if (is_valid_identifier(args[i]))
			ft_unsetenv(args[i]);
		else
			print_invalid_identifier(args[i]);
		i++;
	}
	return (RET_OK);
}
