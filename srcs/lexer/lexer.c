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

int	set_next_token(char **line, t_token **t, t_token **bt, int lt)
{
	int	err;

	if (ft_isoperator(**line))
	{
		if (lt != WORD)
			err = set_bad_token(line, bt);
		else
			err = set_operator_token(line, t);
	}
	else
	{
		err = set_word_token(line, t);
	}
	return (err);
}

int	lex(char *line, t_token **token, t_token **bad_token)
{
	int		last_type;
	int		err;

	g_ms.nb_cmds = 1;
	ft_skipspaces(&line);
	if (*line == '|')
		return (set_bad_token(&line, bad_token));
	last_type = WORD;
	while (*line)
	{
		ft_skipspaces(&line);
		if (*line == '\0')
			break ;
		err = set_next_token(&line, token, bad_token, last_type);
		if (err != 0)
			return (err);
		last_type = (*token)->type;
		token = &((*token)->next);
	}
	if (last_type != WORD)
		return (set_bad_token(&line, bad_token));
	return (LEXER_OK);
}
