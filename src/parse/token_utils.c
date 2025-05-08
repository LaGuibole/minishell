/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:52:23 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/07 16:39:54 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*char	*get_next_token(
	char *str,
	size_t start,
	size_t *consumed,
	t_quote_type *qtype
)
{
	size_t	i;
	char	quote;

	*qtype = NO_QUOTE;
	i = 0;
	if (str[start] == '\'' || str[start] == '"')
	{
		quote = str[start];
		if (quote == '\'')
			*qtype = SINGLE_QUOTE;
		else
			*qtype = DOUBLE_QUOTE;
		start++;
		while (str[start + i] && str[start + i] != quote)
			i++;
		*consumed = i + 2;
		return (ft_substr(str, start, i));
	}
	while (str[start + i] && str[start + i] != ' '
		&& str[start + i] != '\'' && str[start + i] != '"')
		i++;
	*consumed = i;
	return (ft_substr(str, start, i));
}

char	*get_full_token(char *str, size_t *start)
{
	t_quote_type	qtype;
	size_t			consumed;
	t_tokenizer		tokenizer;

	tokenizer.result = ft_strdup("");
	if (!tokenizer.result)
		return (NULL);
	while (str[*start] && str[*start] != ' ')
	{
		tokenizer.chunk = get_next_token(str, *start, &consumed, &qtype);
		if (!tokenizer.chunk)
			return (free(tokenizer.result), NULL);
		if (qtype == SINGLE_QUOTE)
			tokenizer.expanded = ft_strdup(tokenizer.chunk);
		else
			tokenizer.expanded = expand_param(tokenizer.chunk);
		free(tokenizer.chunk);
		if (!tokenizer.expanded)
			return (free(tokenizer.result), NULL);
		my_strcat(&tokenizer.result, tokenizer.expanded);
		free(tokenizer.expanded);
		*start += consumed;
	}
	return (tokenizer.result);
}

t_quote_type	quote_type_of(char c)
{
	if (c == '\'')
		return (SINGLE_QUOTE);
	if (c == '"')
		return (DOUBLE_QUOTE);
	return (NO_QUOTE);
}
*/