#include "minishell.h"

// lexer can return (quote error | malloc error | syntax error)
// syntax error only occur on unknown token
// lexer set a stream of token in minishell global
// lexer does expansion

static void	free_token(t_token *token)
{
	t_token	*next;

	while (token != NULL)
	{
		next = token->next;
		free(token);
		token = next;
	}
}

static void	handle_error(int err, t_token *token)
{
	if (err == SYNTAX_ERROR)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
		ft_putstr_fd(token->value, STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
	}
	else if (err == QUOTE_ERROR)
	{
		ft_putstr_fd("minishell: quotes don't guard\n", STDERR_FILENO);
	}
	else
	{
		ft_putstr_fd("minishell: unexpected error\n", STDERR_FILENO);
	}
}

int	lex(char *line)
{
	int			err;
	t_token		**token;

	token = &(g_minishell.token);
	while (*line)
	{
		ft_skipspaces(&line);
		if (ft_isoperator(*line))
			err = set_operator_token(&line, token);
		else
			err = set_word_token(&line, token);
		if (err)
		{
			free_token(g_minishell.token);
			handle_error(err, *token);
			return (err);
		}
		token = &((*token)->next);
	}
	return (0);
}
