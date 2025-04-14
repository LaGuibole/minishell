/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:15:30 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/14 12:20:19 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

<<<<<<< Updated upstream
#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
=======
#include "libft.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <errno.h>

>>>>>>> Stashed changes

char	*display_prompt(void);
t_list	**ft_envp(char **envp);
char	*ft_getenv(const char *name);
void	ft_setenv(const char *name, const char *value);
void	ft_unsetenv(const char *name);
char	*display_prompt(void);
<<<<<<< Updated upstream

#endif
=======
int		ft_cd(char **args);
int		ft_pwd();

>>>>>>> Stashed changes
