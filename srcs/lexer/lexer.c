#include "minishell.h"

// lexer check for all syntax errors
// lexer set a stream of token in minishell global
// lexer does expansion

/* static void	free_token(t_token *token) */
/* { */
/* 	t_token	*next; */

/* 	while (token != NULL) */
/* 	{ */
/* 		next = token->next; */
/* 		free(token); */
/* 		token = next; */
/* 	} */
/* } */

int	lex(char *line, t_token **token, t_token **bad_token, int old_status)
{
	int		last_type;
	int		err;

	ft_skipspaces(&line);
	if (*line == '|')
		return (set_bad_token(&line, bad_token));
	last_type = WORD;
	while (*line)
	{
		ft_skipspaces(&line);
		if (ft_isoperator(*line))
		{
			if (last_type != WORD)
				err = set_bad_token(&line, bad_token);
			else
				err = set_operator_token(&line, token);
		}
		else
		{
			err = set_word_token(&line, token, old_status);
		}
		if (err != 0)
			return (err);
		last_type = (*token)->type;
		token = &((*token)->next);
	}
	if (last_type != WORD)
		return (set_bad_token(&line, bad_token));
	return (LEXER_OK);
}
