/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:15:30 by guphilip          #+#    #+#             */
/*   Updated: 2025/04/14 17:03:01 by guphilip         ###   ########.fr       */
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
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			*cmd;
	bool			is_builtin;
	char			**params;
	t_redir			*redir;
	struct s_cmd	*next;
	t_link_type		type_link_next;
}	t_cmd;

char	*display_prompt(void);
t_list	**ft_envp(char **envp);
char	*ft_getenv(const char *name);
void	ft_setenv(const char *name, const char *value);
void	ft_unsetenv(const char *name);
int		ft_cd(char **args);
int		ft_pwd(void);
t_cmd	*parsing_cmd(char *str);
int		parse_cmd(char *str, t_cmd *cmd);

#endif
