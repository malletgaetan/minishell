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
	g_minishell.token = NULL;
}

int	interpret(char *line)
{
	int	err;

	if ((err = lex(line, &(g_minishell.token), &(g_minishell.bad_token), g_minishell.old_status)))
	{
		handle_lexer_error(err);
		free_tokens();
		return (2);
	}
	if ((err = ex_cmds()))
		handle_executor_error(err);
	free_tokens();
	return (0);
}

int	interpret_loop(void)
{
	char	*line;
	int	err;
	
	while (42)
	{
		line = readline("minishell$>");
		if (line == NULL)
			break ;
		err = lex(line, &(g_minishell.token), &(g_minishell.bad_token), g_minishell.old_status);
		if (err != LEXER_OK)
		{
			handle_lexer_error(err);
			free_tokens();
			free(line);
			continue ;
		}
		err = ex_cmds();
		if (err != 0) // create EXECUTOR_OK and EXECUTOR_ERRORS...
			handle_executor_error(err);
		free_tokens();
		free(line);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	int	err;

	if (argc == 1)
		err = interpret_loop();
	else
		err = interpret(argv[1]);
	return (err);
}
