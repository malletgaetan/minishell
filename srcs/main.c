/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatteux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 16:28:06 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/20 17:05:27 by tbatteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_minishell	g_minishell;

static int	init_minishell(char **env)
{
	g_minishell.bad_token = NULL;
	g_minishell.token = NULL;
	g_minishell.old_status = 0;
	gc_init(&(g_minishell.gcan));
	gc_init(&(g_minishell.gcenv));
	setup_sigactions();
	if (get_env_copy(env))
		return (1);
	return (0);
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

static int	interpret(char *line)
{
	int	err;

	err = lex(line, &(g_minishell.token), &(g_minishell.bad_token), g_minishell.old_status);
	if (g_minishell.sigint != 0)
		return (130);
	if (err != LEXER_OK)
	{
		handle_lexer_error(err);
		gc_clean(&(g_minishell.gcan));
		gc_clean(&(g_minishell.gcenv));
		return (2);
	}
	if (g_minishell.token == NULL)
		return (0);
	err = exec();
	gc_clean(&(g_minishell.gcan));
	gc_clean(&(g_minishell.gcenv));
	if (err != OK)
	{
		printf("minishell: hardfail error: %s\n", strerror(errno));
		return (1);
	}
	return (0);
}

static int	interpret_loop(void)
{
	char	*line;
	int		err;

	while (42)
	{
		g_minishell.sigint = 0;
		g_minishell.token = NULL;
		g_minishell.status = STATUS_IDLE;
		line = readline("minishell$>");
		g_minishell.status = STATUS_RUNNING;
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
		if (g_minishell.token == NULL)
		{
			free(line);
			continue ;
		}
		add_history(line);
		free(line);
		err = exec();
		gc_clean(&(g_minishell.gcan));
		if (err != OK)
		{
			printf("minishell: hardfail error: %s\n", strerror(errno));
			gc_clean(&(g_minishell.gcenv));
			return (1);
		}
	}
	gc_clean(&(g_minishell.gcenv));
	return (OK);
}

int	main(int argc, char **argv, char **env)
{
	int	err;

	if (init_minishell(env))
	{
		printf("minishell: hardfail error: %s\n", strerror(errno));
		return (1);
	}
	if (argc == 1)
		err = interpret_loop();
	else
		err = interpret(argv[1]);
	return (err);
}
