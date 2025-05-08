/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 23:43:34 by mlintot           #+#    #+#             */
/*   Updated: 2025/05/08 21:18:16 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Adds a redirection entry to the given commands redirection list
///			Handles appending to the end of the existing list
/// @param cmd The command to which the redirection should be added
/// @param type The type of redirection (input, output, append, heredoc)
/// @param filename The target filename for the redirection (must be malloc'd)
/// @return RET_OK on success, RET_ERR on memory allocation failure
int	add_redir(t_cmd *cmd, t_redir_type type, char *filename)
{
	t_redir	*new;
	t_redir	*tmp;

	new = malloc(sizeof(t_redir));
	if (!new)
		return (RET_ERR);
	new->type = type;
	new->filename = filename;
	new->next = NULL;
	if (!cmd->redir)
		cmd->redir = new;
	else
	{
		tmp = cmd->redir;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (RET_OK);
}

/// @brief Checks whether a char is a redir operator ('<' or '>')
/// @param c The char to check
/// @return true if the character is a redirection operator, false otherwise
bool	is_redir_char(char c)
{
	return (c == '<' || c == '>');
}

/// @brief Skips char in a string based on a given type
///			Type 1 skips whitespace; 2 until a space or redir operator
/// @param str The input string
/// @param i The starting index in the string
/// @param type The skip mode: 1 for spaces, 2 for arg scanning
/// @return The number of char skipped
int	ft_skip_char(char *str, int i, int type)
{
	int	tmp;

	tmp = i;
	if (type == 1)
		while (str[i] && str[i] == ' ')
			i++;
	if (type == 2)
		while (str[i] && str[i] != ' ' && str[i] != '<' && str[i] != '>')
			i++;
	return (i - tmp);
}

/// @brief Extract a filename for a redirection from the given string and adds
///		it to the command
/// @param str The command string being parsed
/// @param type The type of redirection being processed
/// @param i The index at which the filename starts
/// @param cmd The command to which the redirection should be added
/// @return RET_OK on success, RET_ERR on failure
int	set_filename(char *str, t_redir_type type, int i, t_cmd *cmd)
{
	char			*filename;
	int				fname_start;

	fname_start = i;
	i += ft_skip_char(str, i, 2);
	filename = ft_substr(str, fname_start, i - fname_start);
	if (!filename)
		return (print_error(ERR_REDIR));
	if (add_redir(cmd, type, filename))
		return (free(filename), print_error(ERR_REDIR));
	return (RET_OK);
}
