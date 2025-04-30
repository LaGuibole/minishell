/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:15:30 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/29 15:26:49 by guphilip         ###   ########.fr       */
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
# define ERR_QUOTE "MISSING QUOTE\n"
# define ERR_PIPE "zsh: parse error near `|'\n"
# define ERR_MALLOC "NOT ENOUGH MEMORY\n"
# define ERR_PARSE "ERROR WHILE PARSING COMMAND\n"
# define ERR_CHEV "bash: syntax error near unexpected token"

extern int	g_signal;

typedef enum e_redir_type
{
	R_NONE,
	R_INPUT,
	R_OUTPUT,
	R_APPEND,
	R_HEREDOC
}	t_redir_type;

typedef enum e_link_type
{
	L_NONE,
	L_PIPE,
	L_BACK
}	t_link_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*filename;
	struct s_redir	*next;
	int				fd;
}	t_redir;

typedef struct s_cmd
{
	char			*cmd;
	bool			is_builtin;
	char			**params;
	int				nbparams;
	int				pid;
	struct s_redir	*redir;
	t_link_type		type_link_next;
	struct s_cmd	*next;
	struct s_cmd	*all_cmds;
}	t_cmd;

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

//CD HELPER
char	*get_home_path(void);
char	*get_oldpwd_path(void);

//HEREDOC HELPER
char	*ft_mktemp(void);
char	*create_heredoc_fd(const char *delimiter);

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
void	wait_children(t_cmd *cmds);
int		exec_pipeline(t_cmd *cmds, char **envp);
void	apply_shell_redirections(t_redir *redir);

//CLEAN && EXIT
void	free_redirections(t_redir *redir_list);
void	free_all_and_exit(t_cmd *cmd, int code);

//PARSING
t_cmd	*parsing_cmd(char *str);
int		parse_cmd(char *str, t_cmd **cmd);
t_cmd	*cmdlast(t_cmd *cmd);
void	cmdadd_back(t_cmd **cmd, t_cmd *new);
t_cmd	*cmdnew(void);
bool	print_error(char *str);
bool	ft_quote(char type, bool upd);
int		add_redir(t_cmd *cmd, t_redir_type type, char *filename);
bool	is_redir_char(char c);
int		set_env_parameters(t_cmd *cmd);
char	*clean_parameters(char *str);
int		set_parameters(char *str, t_cmd *cmd);
void	free_strstr(char **str, int nbr);
int		set_is_builtin(t_cmd *cmd);
int		set_cmd(t_cmd *cmd);

//TEST
void	signal_handler(int signo);
void	handle_sigint(int sig);
void	exec_child_process(t_cmd *cmd, char **envp);
void	apply_heredoc_redirections(t_redir *redir);

void	print_cmd_list(t_cmd *cmd); // debug
void	free_cmd_list(t_cmd *cmd);
void	prepare_heredocs(t_cmd *cmds);
bool	has_output_redirections(t_redir *redir);

void	redirect_input_fd(int input_fd);
void	redirect_output_fd(t_cmd *cmd, int *pipefd, bool has_next, bool has_out_redir);
void	redir_output(char *filename);
void	redir_append(char *filename);
void	handle_input_types(t_redir *redir, int *last_input_fd);

#endif
