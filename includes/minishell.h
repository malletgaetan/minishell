#ifndef MINISHELL_H
# define MINISHELL_H

# define BUF_SIZE 4096
# define MALLOC_ERROR 1
# define PIPE_ERROR 2

# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>
# include <stdbool.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "lexer.h"
# include "libft.h"
# include "ft_printf.h"

enum e_error
{
	OS_ERROR,
};

typedef struct s_minishell
{
	int		old_status;
	t_token	*token;
	t_token *bad_token;
	char		buf[BUF_SIZE];
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

void	handle_lexer_error(int err);
void	handle_executor_error(int err);

// TODO create exector.h ? 
int	fd_manual_pipe(int fdfrom, int fdto, char *delim);
int	pipe_to_file(int fdfrom, char *fileto, int redirtype);
int	file_to_pipe(char *filefrom, int fdto);
int	file_to_pipe(char *filefrom, int fdto);
int	exec_next_cmd(t_token *token, int pipereadfd, int pids[10], int depth);
int	ex_cmds(void);

#endif
