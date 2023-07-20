#include "minishell.h"

int	exec_cmds(void)
{
	int		err;
	uint32_t	i;

	g_minishell.pids = gc_malloc(&(g_minishell.gcan), sizeof(int) * (g_minishell.nb_cmds + 1));
	if (g_minishell.pids == NULL)
		return (HARDFAIL_ERROR);
	ft_memset(g_minishell.pids, 0, sizeof(int) * (g_minishell.nb_cmds + 1));
	err = exec_next_cmd(g_minishell.token, 0, 0);
	if (err == HARDFAIL_ERROR)
		kill_all_childs(SIGKILL, 0);
	i = 0;
	setup_sigaction(&(g_minishell.sa_int), SIGINT, 0, sigint); // NO SA_RESTART
	while (g_minishell.pids[i] != 0)
	{
		if (g_minishell.sigint != 0)
		{
			setup_sigaction(&(g_minishell.sa_int), SIGINT, SA_RESTART, sigint);
			kill_all_childs(SIGKILL, i);
		}
		if (waitpid(g_minishell.pids[i], NULL, 0) == -1)
		{
			setup_sigaction(&(g_minishell.sa_int), SIGINT, SA_RESTART, sigint);
			kill_all_childs(SIGKILL, i);
			continue ;
		}
		++i;
	}
	if (g_minishell.sa_int.sa_flags == 0)
		setup_sigaction(&(g_minishell.sa_int), SIGINT, SA_RESTART, sigint); // back to SA_RESTART
	gc_free(&(g_minishell.gcan), (void **)&(g_minishell.pids));
	return (err);
}

char	*next_cmd_name(void)
{
	t_token	*token;

	token = g_minishell.token;
	while (token != NULL)
	{
		if (token->type == WORD)
			return (token->value);
		else
			token = token->next;
		if (token != NULL)
			token = token->next;
	}
	return (NULL);
}

int	exec(void)
{
	char	*cmd;

	cmd = next_cmd_name();
	// nb_cmds == 1 && is in (export, cd, unset, exit) => exec_simple_builtin
	if (is_unpiped_builtin(cmd))
		return (exec_unpiped_builtin());
	return (exec_cmds());
}