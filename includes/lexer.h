#ifndef LEXER_H
# define LEXER_H

# include <stdlib.h>
# include "libft.h"

enum e_error
{
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

#endif
