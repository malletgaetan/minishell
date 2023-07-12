#ifndef MINISHELL_H
# define MINISHELL_H

# define BUF_SIZE 4096
# define MALLOC_ERROR 1
# define PIPE_ERROR 2

# include <errno.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>
# include <stdbool.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include "libft.h"
# include "ft_printf.h"
# include "gc.h"

// TODO create lexer.h
// ============== LEXER ==============
enum e_lexer_error
{
	LEXER_OK,
	LEXER_MALLOC_ERROR,
	LEXER_SYNTAX_ERROR,
	LEXER_QUOTE_ERROR,
};

enum e_type
{
	WORD,
	PIPE,
	S_REDIR_OUT,
	D_REDIR_OUT,
	S_REDIR_IN,
	D_REDIR_IN,
	BAD,
};

typedef struct s_token
{
	enum e_type		type;
	struct s_token	*next;
	char			value[];
}	t_token;

int		copy_with_expansion(char **line, char *word, int old_status);
char	*get_env_from_line(char **line);
int		set_bad_token(char **line, t_token **token);
int		set_operator_token(char **line, t_token **token);
int		set_word_token(char **line, t_token **token, int old_status);
int		ft_isenv(char c);
int		ft_isspace(char c);
int		ft_isoperator(char c);
int		ft_isquote(char c);
void	ft_skipspaces(char **line);
int		lex(char *line, t_token **token, t_token **bad_token, int old_status);
size_t	get_nb_len(int nb);

// ==================================

enum e_error
{
	OK,
	HARDFAIL_ERROR,
	SOFTFAIL_ERROR,
};

typedef struct s_minishell
{
	int			old_status;
	t_token		*token;
	t_token		*bad_token;
	char		buf[BUF_SIZE];
	t_gcan		gcan;
}	t_minishell;

typedef struct s_cmd
{
	char	**args;
	size_t	arg_len;
	int		pipeout[2];
	int		pipein[2];
	int		redirout_type;
	char	*redirout_file;
}	t_cmd;

extern t_minishell	g_minishell;

// TODO create exector.h
int		close_zero(int *fd);
size_t	get_nb_args(t_token *token);
int		fd_manual_pipe(int fdfrom, int fdto, char *delim);
int		pipe_to_file(int fdfrom, char *fileto, int redirtype);
int		file_to_pipe(char *filefrom, int fdto);
int		file_to_pipe(char *filefrom, int fdto);
int		exec_next_cmd(t_token *token, int pipereadfd, int pids[10], int depth);
int		ex_cmds(void);

#endif
