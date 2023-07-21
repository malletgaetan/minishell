/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bad_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatteux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 16:43:48 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/20 16:43:50 by tbatteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	get_token_size(char *line)
{
	char	*start;

	start = line;
	while ((*line != '\0') && !ft_isspace(*line))
		++line;
	return (line - start);
}

static int	set_newline_token(t_token **token)
{
	(*token) = gc_malloc(&(g_ms.gcan), sizeof(t_token) + (8 * sizeof(char)));
	if ((*token) == NULL)
		return (LEXER_MALLOC_ERROR);
	(*token)->type = BAD;
	(*token)->next = NULL;
	(*token)->value[0] = 'n';
	(*token)->value[1] = 'e';
	(*token)->value[2] = 'w';
	(*token)->value[3] = 'l';
	(*token)->value[4] = 'i';
	(*token)->value[5] = 'n';
	(*token)->value[6] = 'e';
	(*token)->value[7] = '\0';
	return (LEXER_SYNTAX_ERROR);
}

int	set_bad_token(char **line, t_token **token)
{
	size_t	tokensize;
	char	*ptr;

	tokensize = get_token_size(*line);
	if (tokensize == 0)
		return (set_newline_token(token));
	(*token) = gc_malloc(&(g_ms.gcan), sizeof(t_token) + (1 * (tokensize + 1)));
	if ((*token) == NULL)
		return (LEXER_MALLOC_ERROR);
	(*token)->type = BAD;
	(*token)->next = NULL;
	ptr = (*token)->value;
	while ((**line != '\0') && !ft_isspace(**line))
		*(ptr++) = *((*line)++);
	*ptr = '\0';
	return (LEXER_SYNTAX_ERROR);
}
