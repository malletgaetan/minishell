#include "minishell.h"

char	*get_env_from_line(char **line)
{
	size_t	i;
	char	*env;
	char	tmp;

	i = 0;
	while (ft_isenv((*line)[i]))
		++i;
	tmp = (*line)[i];
	(*line)[i] = '\0';
	env = getenv(*line);
	(*line)[i] = tmp;
	(*line) += i;
	return (env);
}

void	ft_skipspaces(char **line)
{
	while (**line && ft_isspace(**line))
		++(*line);
}

size_t	get_nb_len(int nb)
{
	int	c;

	if (nb == 0)
		return (1);
	c = 0;
	if (nb < 0)
		++c;
	while (nb)
	{
		nb /= 10;
		++c;
	}
	return (c);
}
