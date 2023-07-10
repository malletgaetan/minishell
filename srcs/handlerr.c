#include "minishell.h"

void	handle_lexer_error(int err)
{
	if (err == LEXER_MALLOC_ERROR)
		ft_printf("minishell: unexpected error\n");
	else if (err == LEXER_SYNTAX_ERROR)
		ft_printf("minishell: syntax errror near unexpected token `%s'\n", g_minishell.bad_token->value);
	else if (err == LEXER_QUOTE_ERROR)
		ft_printf("minishell: quotes doesn't guard\n");
}

void	handle_executor_error(int err)
{
	ft_printf("minishell: executor error %d\n", err);
}
