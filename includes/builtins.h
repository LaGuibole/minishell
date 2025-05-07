/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 10:33:50 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/06 17:56:38 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

typedef struct s_builtin
{
	char	*name;
	int		(*func)(struct s_exec_ctx *ctx, char **args);
}	t_builtin;

bool	cmd_is_builtin(char *name);

#endif
