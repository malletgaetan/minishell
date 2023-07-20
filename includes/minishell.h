#ifndef MINISHELL_H
# define MINISHELL_H

# define BUF_SIZE 4096
# define _XOPEN_SOURCE 700

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
# include <stdio.h>
# include <stdint.h>
# include "libft.h"
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

enum e_status
{
	STATUS_IDLE,
	STATUS_RUNNING,
};

typedef struct sigaction t_sigaction;

typedef struct s_minishell
{
	int			old_status;
	t_token		*token;
	t_token		*bad_token;
	char		buf[BUF_SIZE];
	t_gcan		gcan;
	t_gcan		gcenv;
	int			*pids;
	int			sigint;
	uint32_t	nb_cmds;
	int			last_pid;
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
	struct sigaction	sa_term;
	enum e_status		status;
	char		**envs;
}	t_minishell;

typedef struct s_cmd
{
	char	**args;
	int		arg_len;
	int		pipeout[2];
	int		pipein[2];
	int		redirout_type;
	char	*redirout_file;
}	t_cmd;

extern t_minishell	g_minishell;

// TODO create exector.h
void	kill_all_childs(int sig, uint32_t start);
void	wait_all_childs(void);
int		close_zero(int *fd);
size_t	get_nb_args(t_token *token);
int		fd_manual_pipe(int fdfrom, int fdto, char *delim);
int		pipe_to_file(int fdfrom, char *fileto, int redirtype);
int		file_to_pipe(char *filefrom, int fdto);
int		file_to_pipe(char *filefrom, int fdto);
int		exec_next_cmd(t_token *token, int pipereadfd, int depth);
int		exec(void);
int		exec_unpiped_builtin(void);
int		exec_piped_builtin(int argc, char **argv);
int		is_unpiped_builtin(char *cmd);
int		is_piped_builtin(char *cmd);

// signals
void	sigint(int code);
void	sigquit(int code);
void	sigterm(int code);
void	setup_sigaction(struct sigaction *sa, int sig, int flags, void (*h)(int));
void	setup_sigactions(void);

// builtin et path
int		cd_builtin(int argc, char **argv);
int		echo_builtin(int argc, char **argv);
int		export_builtin(int argc, char **argv);
int		unset_builtin(int argc, char **argv);
int		env_builtin(void);
void	exit_builtin(void);
int		pwd_builtin(void);
char    **ft_split_path(const char *s, char c);
int		get_env(char **env);
char	*right_path(char *cmd, char **env);
int		get_size(char **env);
char	*gc_strdup(char *str);
#endif
