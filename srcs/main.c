#include "minishell.h"

t_minishell	g_minishell;

static void	free_tokens(void)
{
	t_token	*token;

	while (g_minishell.token != NULL)
	{
		token = g_minishell.token->next;
		free(g_minishell.token);
		g_minishell.token = token;
	}
}

static void	handle_lexer_error(int err)
{
	if (err == LEXER_MALLOC_ERROR)
		ft_printf("minishell: unexpected error\n");
	else if (err == LEXER_SYNTAX_ERROR)
		ft_printf("minishell: syntax errror near unexpected token `%s'\n", g_minishell.bad_token->value);
	else if (err == LEXER_QUOTE_ERROR)
		ft_printf("minishell: quotes doesn't guard\n");
}

int	main(int argc, char **argv)
{
	int	err;

	if (argc == 1)
		return (0);
	if ((err = lex(argv[1], &(g_minishell.token), &(g_minishell.bad_token), g_minishell.old_status)))
	{
		handle_lexer_error(err);
		free_tokens();
		return (2);
	}
	if ((err = ex_cmds()))
		ft_printf("minishell: execution error code=%d\n", err);
	free_tokens();
	return (err);
}
