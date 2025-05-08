/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 22:15:45 by mlintot           #+#    #+#             */
/*   Updated: 2025/05/08 21:12:30 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Prints a syntax error message related to redir misuse
///		Detects invalid sequences like single '<' or '>' at the end of an input
///		or isolated/malformed redirection operators
/// @param c The current character to analyze
/// @param next The next character following 'c'
/// @return RET_ERR if a syntax error is found, RET_OK otherwise
static int	check_more_chev(char c, char next)
{
	if (c == '<' || c == '>' || c == '\0')
	{
		if (c == '\0')
			fd_printf(STDOUT_FILENO, "%s `newline'\n", ERR_CHEV);
		else if ((c == '<' && next == '<') || \
			(c == '>' && next == '>'))
			fd_printf(STDOUT_FILENO, "%s `%c%c'\n", \
ERR_CHEV, c, c);
		else
			fd_printf(STDOUT_FILENO, "%s `%c'\n", ERR_CHEV, c);
		return (RET_ERR);
	}
	return (RET_OK);
}

/// @brief Determines the type of refir based on the current and next char
///			and updates the redirection type accordingly
/// @param type A pointer to the redirection type to set
/// @param c The current character ('<' or '>')
/// @param next The next char following 'c'
/// @return The number of char consumed from the input
static int	set_type_chev(t_redir_type *type, char c, char next)
{
	int	i;

	i = 1;
	if (c == '<' && next == '<' && i++)
		*type = R_HEREDOC;
	else if (c == '>' && next == '>' && i++)
		*type = R_APPEND;
	else if (c == '<')
		*type = R_INPUT;
	else
		*type = R_OUTPUT;
	return (i);
}

/// @brief Parses and sets redirection information from a command string
///		Handles heredoc, input, output, append.
/// @param str The command string to analyze
/// @param cmd The command structure to update with redirection informations
/// @return RET_OK on success, RET_ERR on syntax error or allocation failure
static int	set_redirect(char *str, t_cmd *cmd)
{
	size_t			i;
	t_redir_type	type;

	i = 0;
	while (str[i])
	{
		ft_quote(str[i], 1);
		if (!ft_quote('\'', 0) && !ft_quote('"', 0) && \
			(str[i] == '<' || str[i] == '>'))
		{
			i += set_type_chev(&type, str[i], str[i + 1]);
			i += ft_skip_char(str, i, 1);
			if ((ft_strlen(str) <= i && check_more_chev(str[i], '\0')) || \
				(ft_strlen(str) > i && check_more_chev(str[i], str[i + 1])))
				return (RET_ERR);
			if (set_filename(str, type, i, cmd))
				return (RET_ERR);
			i += ft_skip_char(str, i, 2);
		}
		else
			i++;
	}
	return (RET_OK);
}

/// @brief Splits a pipeline string by unquoted pipe ('|') char into
///		separate command strings.
///		Handles quote context to ignore pipes inside quotes.
/// @param line The array of strings to populate with split commands
/// @param str The raw command line string
/// @return The number of split commands stored in 'line'
static int	split_cmd(char **line, char *str)
{
	char	*tmp;
	int		start;
	int		cpt;
	int		nbline;

	tmp = str;
	start = 0;
	cpt = 0;
	nbline = 0;
	while (*tmp)
	{
		ft_quote(*tmp, 1);
		cpt++;
		if (*tmp == '|' && !ft_quote('\'', 0) && !ft_quote('"', 0))
		{
			line[nbline++] = ft_substr(str, start, cpt - 1);
			start = start + cpt;
			cpt = 0;
		}
		tmp++;
	}
	line[nbline++] = ft_substr(str, start, cpt);
	return (nbline);
}

/// @brief Parses a full command line into a linked list of command structure
///		Handles splitting by pipes, redirections, parameters and builtins
/// @param str The input command line string
/// @param cmd A pointer to the head of the command list (will be allocated)
/// @return RET_OK on success, RET_ERR on failure
int	parse_cmd(char *str, t_cmd **cmd)
{
	char	**line;
	int		nbline;
	int		cpt;
	t_cmd	*current;

	line = malloc(sizeof(char *) * (ft_strlen(str) + 1));
	if (!line)
		return (print_error(ERR_MALLOC));
	nbline = split_cmd(line, str);
	cpt = 0;
	while (cpt < nbline)
	{
		current = cmdnew();
		if (!current)
			return (free_strstr(line, nbline), print_error(ERR_MALLOC));
		current->nbparams = 0;
		if (set_redirect(line[cpt], current) || \
set_parameters(line[cpt], current) || set_cmd(current) || \
set_is_builtin(current))
			return (free_strstr(line, nbline), RET_ERR);
		cmdadd_back(cmd, current);
		cpt++;
	}
	free_strstr(line, nbline);
	return (RET_OK);
}
