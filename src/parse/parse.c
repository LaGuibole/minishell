/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:47:16 by mlintot           #+#    #+#             */
/*   Updated: 2025/05/08 21:04:23 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Manages and queries the current quoting state (single or double quo
/// tes).
///        When `upd` is true, toggles the quote state; otherwise,
///			just returns the state.
/// @param type The quote character to check or update (' or ").
/// @param upd Whether to toggle the quote state (true) or just query it(false)
/// @return true if the specified quote type is currently open, false otherwise.
bool	ft_quote(char type, bool upd)
{
	static bool	single_quote = 0;
	static bool	double_quote = 0;

	if (type == '\'' && !double_quote)
	{
		if (upd)
			single_quote = !single_quote;
		return (single_quote);
	}
	if (type == '"' && !single_quote)
	{
		if (upd)
			double_quote = !double_quote;
		return (double_quote);
	}
	return (0);
}

/// @brief Prints an error message to STDOUT and returns RET_ERR
/// @param str The error message to display
/// @return Always returns RET_ERR
bool	print_error(char *str)
{
	fd_printf(STDOUT_FILENO, str);
	return (RET_ERR);
}

/// @brief Checks for empty pipe links or unsupported '&' char outside of quote
/// @param str The input command line string to validate
/// @return true if a syntax error was found, false otherwise
static bool	empty_link(char *str)
{
	bool	pipe;

	pipe = 0;
	while (*str == ' ')
		str++;
	if (*str == '|')
		return (print_error(ERR_PIPE));
	while (*str)
	{
		ft_quote(*str, 1);
		if (*str == '&' && !ft_quote('\'', 0) && !ft_quote('"', 0))
			return (print_error(ERR_PARSE));
		if (*str == '|' && !ft_quote('\'', 0) && !ft_quote('"', 0))
		{
			if (pipe == 1)
				return (print_error(ERR_PIPE));
			pipe = 1;
		}
		if (*str != '|' && *str != ' ')
			pipe = 0;
		str++;
	}
	return (pipe);
}

/// @brief Checks whether the input contains unclosed single or double quote
/// @param str The input command line string to check
/// @return true if quotes are unbalanced, false otherwise
static bool	unclosed_quotes(char *str)
{
	while (*str)
		ft_quote(*str++, 1);
	if (ft_quote('\'', 0) || ft_quote('"', 0))
		return (print_error(ERR_QUOTE));
	return (ft_quote('\'', 0) || ft_quote('"', 0));
}

/// @brief Parses a raw command line string into a linked list of commands
///			Handles quote validation, pipe syntax, and calls the command parser
/// @param str The raw input string from the user
/// @return A pointer to the parsed command list, or NULL on error
t_cmd	*parsing_cmd(char *str)
{
	t_cmd	*cmd;

	cmd = NULL;
	if (unclosed_quotes(str) || empty_link(str) || parse_cmd(str, &cmd))
	{
		if (ft_quote('\'', 0))
			ft_quote('\'', 1);
		if (ft_quote('"', 0))
			ft_quote('"', 1);
		if (cmd)
			free_cmd_list(cmd);
		g_signal = 2;
		return (NULL);
	}
	return (cmd);
}
