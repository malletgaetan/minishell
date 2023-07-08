#include "minishell.h"
#include <stdio.h>

static enum e_type	redir_out_type(char **line)
{
	if (*((*line) + 1) == '>')
	{
		++(*line);
		return (D_REDIR_OUT);
	}
	return (S_REDIR_OUT);
}

static enum e_type	redir_in_type(char **line)
{
	if (*((*line) + 1) == '<')
	{
		++(*line);
		return (D_REDIR_IN);
	}
	return (S_REDIR_IN);
}

int	set_operator_token(char **line, t_token **token)
{
	enum e_type	type;
	int		err;

	err = 0;
	if ((**line) == '<')
		type = redir_in_type(line);
	else if ((**line) == '>')
		type = redir_out_type(line);
	else if ((**line) == '|')
		type = PIPE;
	else if ((**line) == ')')
		type = C_PARENTHESIS;
	else if ((**line) == '(')
		type = O_PARENTHESIS;
	else
		err = SYNTAX_ERROR;
	++(*line);
	(*token) = malloc(sizeof(t_token) + (sizeof(char) * 3));
	if ((*token) == NULL)
		return (MALLOC_ERROR);
	(*token)->type = type;
	(*token)->next = NULL;
	return (err);
}
