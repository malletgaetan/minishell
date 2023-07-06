#include "minishell.h"

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

size_t	get_expanded_size(char *line)
{
	size_t	size;
	char	*env;
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
		if (*line == '$')
		{
			env = get_env_from_line(&line);
			if (env == NULL)
				continue ;
			size += ft_strlen(env);
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