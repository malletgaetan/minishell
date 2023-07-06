#include "minishell.h"

int	ft_isspace(char c)
{
	return ((c == ' ') || ((c > 8) && (c < 14)));
}

int	ft_isoperator(char c)
{
	return ((c == '<') || (c == '>') || (c == '|') || (c == '&') || (c == '(') || (c == ')'));
}

int	ft_isquote(char c)
{
	return ((c == '\'') || (c == '\"'));
}

int	ft_isdigit(char c)
{
	return ((c >= '0') && (c <= '9'));
}

int	ft_isalpha(char c)
{
	return (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')));
}

int	ft_isenv(char c)
{
	return (ft_isalpha(c) || ft_isdigit(c) || (c == '_'));
}
