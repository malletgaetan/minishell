/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpret.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 08:36:03 by gmallet           #+#    #+#             */
/*   Updated: 2023/07/22 08:36:06 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_lexer_error(int err)
{
	if (err == LEXER_MALLOC_ERROR)
		printf("minishell: unexpected error\n");
	else if (err == LEXER_SYNTAX_ERROR)
	{
		printf("minishell: syntax error");
		printf(" unexpected token `%s'\n", g_ms.bad_token->value);
	}
	else if (err == LEXER_QUOTE_ERROR)
		printf("minishell: quotes doesn't guard\n");
}

void	interpret(char *line)
{
	int	err;

	err = lex(line, &(g_ms.token), &(g_ms.bad_token));
	if (g_ms.sigint != 0)
	{
		g_ms.old_status = 130;
		return ;
	}
	if (err != LEXER_OK)
	{
		handle_lexer_error(err);
		gc_clean(&(g_ms.gcan));
		gc_clean(&(g_ms.gcenv));
		g_ms.old_status = 2;
		return ;
	}
	if (g_ms.token == NULL)
	{
		g_ms.old_status = 0;
		return ;
	}
	exec();
	g_ms.old_status = WEXITSTATUS(g_ms.old_status);
}

static char	*get_next_line(void)
{
	char	*line;

	while (42)
	{
		g_ms.sigint = 0;
		g_ms.token = NULL;
		g_ms.status = STATUS_IDLE;
		line = readline("minishell$>");
		if (g_ms.sigint != 0)
		{
			free(line);
			continue ;
		}
		g_ms.status = STATUS_RUNNING;
		return (line);
	}
}

static int	local_lex(char *line)
{
	int	err;

	err = lex(line, &(g_ms.token), &(g_ms.bad_token));
	if (err != LEXER_OK)
	{
		handle_lexer_error(err);
		g_ms.old_status = 2;
		return (1);
	}
	return (0);
}

void	interpret_loop(void)
{
	char	*line;

	line = NULL;
	while (42)
	{
		gc_clean(&(g_ms.gcan));
		free(line);
		line = get_next_line();
		if (line == NULL)
			break ;
		if (local_lex(line))
			continue ;
		if (g_ms.token == NULL)
		{
			g_ms.old_status = 0;
			continue ;
		}
		add_history(line);
		exec();
		g_ms.old_status = WEXITSTATUS(g_ms.old_status);
	}
	free(line);
}
