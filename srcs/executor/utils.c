#include "minishell.h"

void	wait_all_childs(void)
{
	uint32_t	i;
	
	i = 0;
	if (g_minishell.pids == NULL)
		return ;
	while (g_minishell.pids[i] != 0)
		waitpid(g_minishell.pids[i++], NULL, 0);
}

void	kill_all_childs(int sig, uint32_t start)
{
	uint32_t	i;

	if (g_minishell.pids == NULL)
		return ;
	i = start;
	while (g_minishell.pids[i] != 0)
		kill(g_minishell.pids[i++], sig);
}

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
		if (token->type == WORD)
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
