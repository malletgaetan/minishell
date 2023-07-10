#include "lexer.h"

static enum e_type	redir_out_type(char **line)
{
	if (*((*line) + 1) == '>')
		return (D_REDIR_OUT);
	return (S_REDIR_OUT);
}

static enum e_type	redir_in_type(char **line)
{
	if (*((*line) + 1) == '<')
		return (D_REDIR_IN);
	return (S_REDIR_IN);
}

static void	cpytoken(char *to, char **line)
{
	while (ft_isoperator(**line))
		*(to++) = *((*line)++);
	*to = '\0';
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
	else
		err = LEXER_SYNTAX_ERROR;
	(*token) = malloc(sizeof(t_token) + (sizeof(char) * 3));
	if ((*token) == NULL)
		return (LEXER_MALLOC_ERROR);
	cpytoken((*token)->value, line);
	(*token)->type = type;
	(*token)->next = NULL;
	return (err);
}