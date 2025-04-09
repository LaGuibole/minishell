/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:15:30 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/09 15:11:21 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

char	*display_prompt(void);
t_list	**ft_envp(char **envp);
char	*ft_getenv(const char *name);
void	ft_setenv(const char *name, const char *value);
void	ft_unsetenv(const char *name);
char	*display_prompt(void);
