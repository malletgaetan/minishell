#include "minishell.h"

static int	cpy_squote(char **line, char **word)
{
	while (**line != '\'')
	{
		if (**line == '\0')
			return (QUOTE_ERROR);
		*(++(*word)) = *(++(*line));
	}
	++(*line);
	return (0);
}

static int	copy_with_expansion(char **line, char *word)
{
	char	*env;
	int		in_dquote;

	in_dquote = 0;
	while ((**line != '\0') && !(!in_dquote && (ft_isoperator(**line) || ft_isspace(**line))))
	{
		if (((**line == '\'') && !in_dquote))
		{
			if (cpy_squote(line, &word))
				return (QUOTE_ERROR);
			continue ;
		}
		if (**line == '\"')
		{
			in_dquote = !in_dquote;
			++(*line);
			continue ;
		}
		if (**line == '$')
		{
			env = get_env_from_line(line);
			if (env != NULL)
			{
				while (*env)
					*(word++) = *(env++);
				continue ;
			}
		}
		*word = **line;
		++(*line);
		++(word);
	}
	*word = '\0';
	if (in_dquote)
		return (QUOTE_ERROR);
	return (0);
}

int	set_word_token(char **line, t_token **token)
{
	size_t	word_size;

	word_size = get_expanded_size(*line);
	(*token) = malloc(sizeof(t_token) + (sizeof(char) * (word_size + 1)));
	(*token)->next = NULL;
	(*token)->type = WORD;
	if ((*token) == NULL)
		return (MALLOC_ERROR);
	if (copy_with_expansion(line, (*token)->value))
		return (QUOTE_ERROR);
	return (0);
}