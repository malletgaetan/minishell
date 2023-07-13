#include "minishell.h"

int	ex_cmds(void)
{
	int	pids[10] = {0};
	t_token	*token;
	int	err;
	int	i;

	token = g_minishell.token;
	err = exec_next_cmd(token, 0, pids, 0);
	if (err == HARDFAIL_ERROR)
	{
		i = 0;
		while ((i < 10) && (pids[i] != 0))
			kill(pids[i++], SIGKILL);
	}
	i = 0;
	while ((i < 10) && (pids[i] != 0))
		waitpid(pids[i++], NULL, 0);
	return (err);
}
