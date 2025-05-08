/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:15:30 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/08 22:11:46 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
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
# define ERR_REDIR "ERROR WITH < / >\n"
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

typedef enum e_quote_type
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
}	t_quote_type;

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

typedef struct s_tokenizer
{
	char			*chunk;
	char			*expanded;
	char			*result;
	char			*tmp;
}	t_tokenizer;

typedef struct s_exec_ctx
{
	t_cmd	*curr;
	t_cmd	*head;
}	t_exec_ctx;

// BUILTINS
int				ft_cd(t_exec_ctx *ctx, char **args);
int				ft_pwd(t_exec_ctx *ctx, char **args);
int				ft_echo(t_exec_ctx *ctx, char **args);
int				ft_export(t_exec_ctx *ctx, char **args);
int				ft_env(t_exec_ctx *ctx, char **args);
int				ft_unset(t_exec_ctx *ctx, char **args);
int				ft_exit(t_exec_ctx *ctx, char **args);
void			print_invalid_identifier(char *arg);
int				process_export_arg(char *arg);
void			print_export_var(char *var);

// CLEAN
void			free_strstr(char **str, int nbr);
void			free_redir(t_redir *redir);
void			free_cmd(t_cmd *cmd);
void			free_cmd_list(t_cmd *cmd);
void			exit_child(t_cmd *cmd, int status);

// CORE
char			*display_prompt(void);

// ENV
bool			match_env_entry(const char *entry, const char *name);
void			swap_nodes(t_list *a, t_list *b);
t_list			*copy_env_list(void);
int				sort_env_list_part(
					t_list *sorted_env, t_list **current, t_list *last);
void			sort_env_list(t_list *sorted_env);
void			free_array(char **array);
char			**env_to_array(void);
t_list			**ft_envp(char **envp);
char			*ft_getenv(const char *name);
void			ft_setenv(const char *name, const char *value);
void			ft_unsetenv(const char *name);

// PARSING
t_cmd			*parsing_cmd(char *str);
int				parse_cmd(char *str, t_cmd **cmd);
t_cmd			*cmdlast(t_cmd *cmd);
void			cmdadd_back(t_cmd **cmd, t_cmd *new);
t_cmd			*cmdnew(void);
bool			print_error(char *str);
bool			ft_quote(char type, bool upd);
int				add_redir(t_cmd *cmd, t_redir_type type, char *filename);
bool			is_redir_char(char c);
char			*clean_parameters(char *str);
int				set_parameters(char *str, t_cmd *cmd);
int				set_is_builtin(t_cmd *cmd);
int				set_cmd(t_cmd *cmd);
int				ft_skip_char(char *str, int i, int type);
int				set_filename(char *str, t_redir_type type, int i, t_cmd *cmd);
void			ft_strcat_free(char **dst_ptr, char *src);
int				set_expand(t_cmd *line);

// EXEC
bool			cmd_is_builtin(char *name);
bool			is_parent_builtin(char *cmd);
bool			is_builtin_pipeable(const char *name);
int				exec_builtin(t_exec_ctx *ctx);
bool			skip_empty_cmd(t_cmd **curr, int *input_fd);
int				setup_pipe(int pipefd[2], bool has_next);
int				exec_pipeline(t_exec_ctx *ctx);
void			setup_pipe_redirections(
					int input_fd, int *pipefd, bool has_next);
void			exec_child_process(t_exec_ctx *ctx);
int				validate_redirections(t_cmd *cmds);
void			apply_shell_redirections(t_redir *redir);
bool			has_output_redirections(t_redir *redir);
void			redirect_input_fd(int input_fd, int *pipefd);
void			redirect_output_fd(
					t_cmd *cmd, int *pipefd, bool has_next, bool has_out_redir);
pid_t			fork_child(t_exec_ctx *ctx, int input_fd, int *pipefd);
int				parent_cleanup(int input_fd, int *pipefd, bool has_next);
void			wait_children(t_cmd *cmds);
void			handle_input_types(t_redir *redir, int *last_input_fd);
void			redir_output(char *filename);
void			redir_append(char *filename);
int				exec_cmd(t_exec_ctx *ctx);

// PATHING
char			*get_cmd_path(t_cmd *cmd, char **envp);
char			*get_env_path(char **envp);
char			**get_paths(char **envp);
char			*join_cmd_paths(char *path, char *cmd);

// UTILS
char			*get_home_path(void);
char			*get_oldpwd_path(void);
bool			is_valid_identifier(const char *name);
int				process_export_no_equal(char *arg);
int				process_export_with_equal(char *arg, char *equal_sign);
void			print_and_free_sorted_env(t_list *sorted_env);
void			print_sorted_env(void);
bool			match_env_to_delete(char *entry, const char *name, size_t len);
void			remove_env_node(t_list **head, t_list *curr, t_list *prev);
void			free_redirections(t_redir *redir_list);
void			free_all_and_exit(t_cmd *cmd, int code);
void			print_invalid_identifier(char *arg);
char			*create_heredoc_fd(const char *delimiter);
void			prepare_heredocs(t_cmd *cmds);
char			*ft_mktemp(bool res);
void			set_sig_executing(void);

// ?? 
void			handle_signal_interactive(int sig);
void			handle_signal_executing(int sig);
void			apply_heredoc_redirections(t_redir *redir);
void			print_cmd_list(t_cmd *cmd); // debug
bool			is_exported(const char *name);
int				mark_for_export(const char *name);
void			remove_from_export_list(const char *name);
char			*expand_param(char *param);
int				set_expanded_param(
					char **params, char *str, t_cmd *cmd, size_t *start);
char			*get_next_token(
					char *str,
					size_t start,
					size_t *consumed,
					t_quote_type *qtype);
char			*get_full_token(char *str, size_t *start);

t_quote_type	quote_type_of(char c);

#endif
