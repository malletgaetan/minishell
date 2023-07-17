#include "minishell.h"

t_minishell	g_minishell;


static void	init_minishell(void)
{
	g_minishell.bad_token = NULL;
	g_minishell.token = NULL;
	g_minishell.old_status = 0;
	gc_init(&(g_minishell.gcan));
}

void	handle_lexer_error(int err)
{
	if (err == LEXER_MALLOC_ERROR)
		printf("minishell: unexpected error\n");
	else if (err == LEXER_SYNTAX_ERROR)
		printf("minishell: syntax errror near unexpected token `%s'\n", g_minishell.bad_token->value);
	else if (err == LEXER_QUOTE_ERROR)
		printf("minishell: quotes doesn't guard\n");
}

static int	interpret(char *line, char **env)
{
	int	err;

	err = lex(line, &(g_minishell.token), &(g_minishell.bad_token), g_minishell.old_status);
	if (g_minishell.sigint != 0)
		return (130);
	if (err != LEXER_OK)
	{
		handle_lexer_error(err);
		gc_clean(&(g_minishell.gcan));
		return (2);
	}
	if (g_minishell.token == NULL)
		return (0);
	err = exec();
	gc_clean(&(g_minishell.gcan));
	if (err != OK)
	{
		printf("minishell: hardfail error: %s\n", strerror(errno));
		return (1);
	}
	return (0);
}

static int	interpret_loop(char **env)
{
	char	*line;
	int	err;
	
	while (42)
	{
		g_minishell.sigint = 0;
		g_minishell.token = NULL;
		line = readline("minishell$>");
		if (line == NULL)
			break ;
		if (g_minishell.sigint != 0)
		{
			free(line);
			continue ;
		}
		err = lex(line, &(g_minishell.token), &(g_minishell.bad_token), g_minishell.old_status);
		if (err != LEXER_OK)
		{
			handle_lexer_error(err);
			gc_clean(&(g_minishell.gcan));
			free(line);
			continue ;
		}
		free(line);
		if (g_minishell.token == NULL)
			continue ;
		err = exec(); 
		gc_clean(&(g_minishell.gcan));
		if (err != OK)
		{
			printf("minishell: hardfail error: %s\n", strerror(errno));
			return (1);
		}
	}
	return (OK);
}

int	main(int argc, char **argv, char **env)
{
	int	err;

	init_minishell();
	if (argc == 1)
		err = interpret_loop(env);
	else
		err = interpret(argv[1], env);
	return (err);
}
