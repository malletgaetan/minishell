#ifndef MINISHELL_H
# define MINISHELL_H

# define BUF_SIZE 4096
# define MALLOC_ERROR 1
# define QUOTE_ERROR 2
# define SYNTAX_ERROR 3

# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>
# include <stdbool.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "lexer.h"

// TODO DONT FORGET TO DELETE THIS
# include <stdio.h>

typedef struct s_minishell
{
	int		old_status;
	t_token	*token;
	t_token *bad_token;
	char		buf[BUF_SIZE];
}	t_minishell;

typedef struct s_cmd
{
	size_t	arg_len;
	char	**args;
	int		fds[2];
}	t_cmd;

extern t_minishell	g_minishell;

#endif
