#ifndef MINISHELL_H
# define MINISHELL_H

# define BUF_SIZE 4096
# define MALLOC_ERROR 1
# define QUOTE_ERROR 2
# define SYNTAX_ERROR 3


# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <stdio.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

enum e_type
{
	WORD,
	PIPE,
	S_REDIR_OUT,
	D_REDIR_OUT,
	S_REDIR_IN,
	D_REDIR_IN,
	C_PARENTHESIS,
	O_PARENTHESIS,
};
typedef struct s_token
{
	enum e_type		type;
	struct s_token	*next;
	char			value[];
}	t_token;

// !!! TODO LIBFT !!!
int		ft_isdigit(char c);
int		ft_isalpha(char c);
size_t	ft_strlen(char *str);
void	ft_putstr_fd(char *str, int fd);
char	*ft_itoa(int nb);
// !!!!!!!!!!!!!!!!!!

size_t	get_expanded_size(char *line);
char	*get_env_from_line(char **line);
int		set_operator_token(char **line, t_token **token);
int		set_word_token(char **line, t_token **token);
int		ft_isenv(char c);
int		ft_isspace(char c);
int		ft_isoperator(char c);
int		ft_isquote(char c);
void	ft_skipspaces(char **line);
int		lex(char *line);

typedef struct s_minishell
{
	int		status;
	t_token	*token;
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
