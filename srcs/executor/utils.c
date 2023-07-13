#include "minishell.h"

int	close_zero(int *fd)
{
	if (*fd == 0)
		return (OK);
	if (close(*fd))
		return (HARDFAIL_ERROR);
	*fd = 0;
	return (OK);
}

size_t	get_nb_args(t_token *token)
{
	size_t	c;

	c = 0;
	while (token && token->type != PIPE)
	{
		if (token == WORD)
		{
			token = token->next;
			++c;
			continue ;
		}
		token = token->next;
		if (token)
			token = token->next;
	}
	return (c);
}