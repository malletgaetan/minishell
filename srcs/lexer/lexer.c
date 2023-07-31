/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatteux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 16:45:06 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/20 16:45:07 by tbatteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_next_token(char **line, t_token **t)
{
	int	err;

	if (ft_isoperator(**line))
		err = set_operator_token(line, t);
	else
		err = set_word_token(line, t);
	return (err);
}

int	bad_syntax(int last_type, int new_type)
{
	if ((last_type == PIPE || last_type == -1) && new_type == PIPE)
		return (1);
	if ((last_type != PIPE && last_type != WORD) && \
		(new_type != PIPE && new_type != WORD))
		return (1);
	return (0);
}

int	lex(char *line, t_token **token, t_token **bad_token)
{
	int		last_type;
	int		err;

	g_ms.nb_cmds = 1;
	last_type = -1;
	while (*line)
	{
		ft_skipspaces(&line);
		if (*line == '\0')
			break ;
		err = set_next_token(&line, token);
		if (err != 0)
			return (err);
		if (bad_syntax(last_type, (*token)->type))
		{
			(*token)->type = BAD;
			(*bad_token) = (*token);
			return (LEXER_SYNTAX_ERROR);
		}
		last_type = (*token)->type;
		token = &((*token)->next);
	}
	if (last_type != WORD)
		return (set_bad_token(&line, bad_token));
	return (LEXER_OK);
}
