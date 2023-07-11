#include "minishell.h"

void	handle_soft_error(char *filename)
{
	ft_printf("minishell: %s: %s\n", strerror(errno), filename);
}

void	handle_hard_error(void)
{
	int	i;

	i = 0;
	while ((i < 10) && (pids[i] != 0))
		kill(pids[i++], SIGKILL);
	ft_printf("minishell: unexpected error: %s\n", strerror(errno));
}

int	ex_cmds(void)
{
	int	pids[10] = {0};
	t_token	*token;
	int	err;
	int	i;

	token = g_minishell.token;
	err = exec_next_cmd(token, 0, pids, 0);
	handle_exec_error(err);
	if (err == HARDFAIL_ERROR)
		handle_hard_error();
	i = 0;
	while ((i < 10) && (pids[i] != 0))
		waitpid(pids[i++], NULL, 0);
	return (err);
}
