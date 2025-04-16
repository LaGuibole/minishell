/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:15:30 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/16 18:54:27 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <errno.h>

# define RET_OK 0
# define RET_ERR 1

// PROMPT
char	*display_prompt(void);

// ENV
t_list	**ft_envp(char **envp);
char	*ft_getenv(const char *name);
void	ft_setenv(const char *name, const char *value);
void	ft_unsetenv(const char *name);

// BUILTINS
int		ft_cd(char **args);
int		ft_pwd(void);
int		ft_echo(char **args);
int		ft_export(char **args);
void	ft_env(void);
int		ft_unset(char **args);

// EXPORT HELPERS
void	print_invalid_identifier(char *arg);
int		process_export_no_equal(char *arg);
int		process_export_with_equal(char *arg, char *equal_sign);
void	print_and_free_sorted_env(t_list *sorted_env);
void	print_sorted_env(void);
int		process_export_arg(char *arg);
void	print_export_var(char *var);

// UNSET HELPERS
bool	match_env_to_delete(char *entry, const char *name, size_t len);
void	remove_env_node(t_list **head, t_list *curr, t_list *prev);

// ENV HELPERS
bool	match_env_entry(const char *entry, const char *name);
void	swap_nodes(t_list *a, t_list *b);
t_list	*copy_env_list(void);
int		sort_env_list_part(t_list *sorted_env, t_list **current, t_list *last);
void	sort_env_list(t_list *sorted_env);
bool	is_valid_identifier(const char *name);

// ERRORS
void	print_invalid_identifier(char *arg);

#endif
