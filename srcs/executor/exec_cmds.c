#include "minishell.h"

void	kill_all_childs(void)
{
	uint_t	i;

	i = 0;
	while (g_minishell.pids[i] != 0)
		kill(g_minishell.pids[i++], SIGKILL);
}

int	ex_cmds(void)
{
	int		status;
	int		pid;
	int		err;
	uint_t	i;

	g_minishell.nb_child = 0;
	g_minishell.pids = gc_malloc(&(g_minishell.gcan), sizeof(int) * (g_minishell.nb_cmds + 1));
	if (g_minishell.pids == NULL)
		return (HARDFAIL_ERROR);
	ft_memset(g_minishell.pids, 0, sizeof(int) * (g_minishell.nb_cmds + 1));
	err = exec_next_cmd(g_minishell.token, 0, pids, 0);
	if (err == HARDFAIL_ERROR)
		kill_all_childs();
	i = 0;
	while (g_minishell.pids[i] != 0)
	{
		waitpid(g_minishell.pids[i], g_minishell.pids + i, 0);
		++i;
	}
	g_minishell.old_status = g_minishell.pids[i - 1]; // TODO make sure there is always at least one cmd to execute in lexer
	gc_free(&(g_minishell.gcan), (void **)&(g_minishell.pids));
	return (err);
}
