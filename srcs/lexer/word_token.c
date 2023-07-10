#include "lexer.h"

// TODO update it with new env variable array system

static size_t	get_single_quote_len(char **line)
{
	size_t	size;

	++(*line);
	size = 0;
	while (**line)
	{
		if ((**line) == '\'')
		{
			++(*line);
			break ;
		}
		++(*line);
		++size;
	}
	return (size);
}

static size_t	get_env_size(char **line, int old_status)
{
	++(*line);
	if ((**line) == '?')
	{
		++(*line);
		return (get_nb_len(old_status));
	}
	return (ft_strlen(get_env_from_line(line)));
}

static size_t	get_expanded_size(char *line, int old_status)
{
	size_t	size;
	int		in_dquote;

	size = 0;
	in_dquote = 0;
	while (*line && !(!in_dquote && (ft_isoperator(*line) || ft_isspace(*line))))
	{
		if ((*line == '\'') && !in_dquote)
		{
			size += get_single_quote_len(&line);
			continue ;
		}
		if (*line == '$' && ft_isenv(*(line + 1)))
		{
			size += get_env_size(&line, old_status);
			continue ;
		}
		if (*line == '\"')
			in_dquote = !in_dquote;
		else
			++size;
		++line;
	}
	return (size);
}

int	set_word_token(char **line, t_token **token, int old_status)
{
	size_t	word_size;

	word_size = get_expanded_size(*line, old_status);
	(*token) = malloc(sizeof(t_token) + (sizeof(char) * (word_size + 1)));
	(*token)->next = NULL;
	(*token)->type = WORD;
	if ((*token) == NULL)
		return (LEXER_MALLOC_ERROR);
	return (copy_with_expansion(line, (*token)->value, old_status));
}