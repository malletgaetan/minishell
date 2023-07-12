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
		ft_printf("minishell: unexpected error\n");
	else if (err == LEXER_SYNTAX_ERROR)
		ft_printf("minishell: syntax errror near unexpected token `%s'\n", g_minishell.bad_token->value);
	else if (err == LEXER_QUOTE_ERROR)
		ft_printf("minishell: quotes doesn't guard\n");
}

static int	interpret(char *line)
{
	int	err;

	err = lex(line, &(g_minishell.token), &(g_minishell.bad_token), g_minishell.old_status);
	if (err != LEXER_OK)
	{
		handle_lexer_error(err);
		gc_clean(&(g_minishell.gcan));
		return (2);
	}
	err = ex_cmds();
	gc_clean(&(g_minishell.gcan));
	if (err != OK)
	{
		ft_printf("minishell: hardfail error: %s\n", strerror(errno));
		return (1);
	}
	return (0);
}

static int	interpret_loop(void)
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
			gc_clean(&(g_minishell.gcan));
			free(line);
			continue ;
		}
		free(line);
		err = ex_cmds(); 
		gc_clean(&(g_minishell.gcan));
		if (err != OK)
		{
			ft_printf("minishell: hardfail error: %s\n", strerror(errno));
			return (1);
		}
	}
	return (OK);
}

int	main(int argc, char **argv)
{
	int	err;

	init_minishell();
	if (argc == 1)
		err = interpret_loop();
	else
		err = interpret(argv[1]);
	return (err);
}
