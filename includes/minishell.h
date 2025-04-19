/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:15:30 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/19 18:17:02 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "builtins.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <errno.h>
# include <fcntl.h>

# define RET_OK 0
# define RET_ERR 1

typedef enum e_redir_type
{
	R_INPUT,
	R_OUTPUT,
	R_APPEND,
	R_HEREDOC
}	t_redir_type;

typedef enum e_link_type
{
	L_PIPE,
	L_BACK
}	t_link_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*filename;
}	t_redir;

typedef struct s_cmd
{
	char			*cmd;
	bool			is_builtin;
	char			**params;
	t_list			*redir;
	t_link_type		type_link_next;
	struct s_cmd	*next;
}	t_cmd;

// PROMPT
char	*display_prompt(void);

// ENV
t_list	**ft_envp(char **envp);
char	*ft_getenv(const char *name);
void	ft_setenv(const char *name, const char *value);
void	ft_unsetenv(const char *name);

// BUILTINS
int		ft_cd(char **args);
int		ft_pwd(char **args);
int		ft_echo(char **args);
int		ft_export(char **args);
int		ft_env(char **args);
int		ft_unset(char **args);

//EXEC BUILTINS
bool	is_parent_builtin(char *cmd);
bool	is_builtin_pipeable(const char *name);
int		exec_builtin(t_cmd *cmd);

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

//HEREDOC HELPER
char	*ft_mktemp(void);
int		create_heredoc_fd(const char *delimiter);

// ERRORS
void	print_invalid_identifier(char *arg);

// PATHING
char	*get_env_path(char **envp);
char	**get_paths(char **envp);
char	*join_cmd_paths(char *path, char *cmd);
char	*get_cmd_path(t_cmd *cmd, char **envp);

//DUMMY
int		exec_cmd(t_cmd *cmd, char **envp);

//PIPELINE EXEC
void	setup_pipe_redirections(int input_fd, int *pipefd, bool has_next);
pid_t	fork_child(t_cmd *cmd, int input_fd, int *pipefd, char **envp);
int		parent_cleanup(int input_fd, int *pipefd, bool has_next);
void	wait_children(void);
int		exec_pipeline(t_cmd *cmds, char **envp);
void	apply_shell_redirections(t_list *redir);

#endif
