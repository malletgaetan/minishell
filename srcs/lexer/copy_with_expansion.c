#include "minishell.h"

// TODO update it with new env variable array system

static int	cpy_squote(char **line, char **word)
{
	while (**line != '\'')
	{
		if (**line == '\0')
			return (LEXER_QUOTE_ERROR);
		*(++(*word)) = *(++(*line));
	}
	++(*line);
	return (LEXER_OK);
}

static uint_t	atoi_buf(char *str, unsigned int old_status)
{
	uint_t	res;

	if (old_status < 10)
	{
		*str = (old_status % 10) + '0';
		return (1);
	}
	res = atoi_buf(str, old_status / 10);
	str[res] = (old_status % 10) + '0';
	return (res + 1);
}

static uint_t	cpy_old_status(char *str, int old_status)
{
	if (old_status < 0)
	{
		*(str++) = '-';
		return (atoi_buf(str, -old_status) + 1);
	}
	return ((atoi_buf(str, old_status)));
}

static int	cpy_env(char *env, char **word)
{
	if (env == NULL)
		return (1);
	while (*env)
		*((*word)++) = *(env++);
	return (OK);
}

int	copy_with_expansion(char **line, char *word, int old_status)
{
	int		in_dquote;

	in_dquote = 0;
	while ((**line != '\0') && !(!in_dquote && (ft_isoperator(**line) || ft_isspace(**line))))
	{
		if (((**line == '\'') && !in_dquote))
		{
			if (cpy_squote(line, &word))
				return (LEXER_QUOTE_ERROR);
			continue ;
		}
		if (**line == '\"')
		{
			in_dquote = !in_dquote;
			++(*line);
			continue ;
		}
		if (**line == '$' && ft_isenv(*((*line) + 1)))
		{
			++(*line);
			if ((**line) == '?')
			{
				++(*line);
				word += cpy_old_status(word, old_status);
				continue ;
			}
			if (!(cpy_env(get_env_from_line(line), &word)))
				continue;
		}
		*word = **line;
		++(*line);
		++(word);
	}
	*word = '\0';
	if (in_dquote)
		return (LEXER_QUOTE_ERROR);
	return (LEXER_OK);
}
