#include "minishell.h"

int	sigint(int code)
{
	(void)code;
	kill_all_childs();
	g_minishell.sigint = 1;
}

int	sigquit(int code)
{
	(void)code;
}
