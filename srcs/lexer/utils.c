#include "minishell.h"

char	*get_env_from_line(char **line)
{
	size_t	i;
	char	*env;
	char	tmp;

	(*line)++;
	if (**line == '?')
	{
		++(*line);
		return (ft_itoa(g_minishell.status));
	}
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


// TODO LIBFT PLEASE
size_t	ft_strlen(char *str)
{
	size_t	c;

	c = 0;
	while (str[c])
		++c;
	return (c);
}

void	ft_putstr_fd(char *str, int fd)
{
	write(fd, str, ft_strlen(str));
}

static size_t	nb_len(int nb)
{
	int	c;

	if (nb == 0)
		return (1);
	c = 0;
	if (nb < 0)
		c++;
	while (nb)
	{
		nb /= 10;
		c++;
	}
	return (c);
}

static void	fill_number(char *buf, unsigned int bufsize, unsigned int nb)
{
	while (bufsize--)
	{
		buf[bufsize] = ((nb % 10) + '0');
		nb /= 10;
	}
}

char	*ft_itoa(int nb)
{
	unsigned int	nblen;
	char			*ptr;

	nblen = nb_len(nb);
	ptr = (char *)malloc(sizeof(char) * (nblen + 1));
	if (!ptr)
		return (NULL);
	ptr[nblen] = '\0';
	if (nb < 0)
	{
		*ptr = '-';
		fill_number(ptr + 1, nblen - 1, (unsigned int)-nb);
	}
	else
	{
		fill_number(ptr, nblen, (unsigned int)nb);
	}
	return (ptr);
}