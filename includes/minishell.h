/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatteux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 16:18:25 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/20 16:18:31 by tbatteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	SOFTFAIL_ERROR,
};

enum e_status
{
	STATUS_IDLE,
	STATUS_RUNNING,
};

typedef struct sigaction	t_sigaction;

typedef struct s_minishell
{
	int					old_status;
	int					sigint;
	int					*pids;
	int					last_pid;
	int					nb_cmds;
	char				buf[BUF_SIZE];
	char				**envs;
	t_token				*token;
	t_token				*bad_token;
	t_gcan				gcan;
	t_gcan				gcenv;
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
	struct sigaction	sa_term;
	enum e_status		status;
}	t_minishell;

typedef struct s_cmd
{
	char	*executable;
	char	**args;
	int		arg_len;
	int		pipeout[2];
	int		pipein[2];
	int		redirout_type;
	char	*redirout_file;
	char	*redirin_file;
}	t_cmd;

extern t_minishell			g_ms;

// main
void	interpret(char *line);
void	interpret_loop(void);

// TODO create exector.h
void	kill_all_childs(int sig);
void	wait_all_childs(void);
void	safe_close(int *fd);
void	safe_pipe(int pids[2]);
void	safe_dup2(int oldfd, int newfd);
int		is_opened_fd(int fd);
size_t	get_nb_args(t_token *token);
void	fd_m_pipe(int fdfrom, int fdto, char *delim);
int		pipe_tofile(int fdfrom, char *fileto, int redirtype);
int		file_to_pipe(char *filefrom, int fdto);
int		file_to_pipe(char *filefrom, int fdto);
void	exec_next_cmd(t_token *token, int pipereadfd, int depth);
void	exec(void);
void	exec_unpiped_builtin(void);
void	exec_builtin(int argc, char **argv);
int		is_unpiped_builtin(char *cmd);
void	hardfail_exit(int lerrno);
void	clean_exit(int err);
int		is_builtin(char *cmd);
int		setup_next_cmd(t_cmd *cmd, t_token **token);
void	setup_child_pipes(t_cmd *cmd, int is_last_cmd, int *pipereadfd);
void	unsetup_child_pipes(t_cmd *cmd, int *pipereadfd);

// signals
void	sigint(int code);
void	sigquit(int code);
void	sigterm(int code);
void	setup_sigaction(t_sigaction *sa, int sig, int flags, void (*h)(int));
void	setup_sigactions(void);

// builtin
int		cd_builtin(int argc, char **argv);
int		echo_builtin(int argc, char **argv);
int		export_builtin(int argc, char **argv);
int		unset_builtin(int argc, char **argv);
int		env_builtin(void);
void	exit_builtin(void);
int		pwd_builtin(void);
char	**ft_split_path(const char *s, char c);
char	*right_path(char *cmd, char **env);
int		get_size(char **env);
char	*gc_strdup(char *str);

// envs
int		get_env_copy(char **env);
char	*ms_getenv(char *envname);
#endif
