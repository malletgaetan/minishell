#include "minishell.h"

int	exec_piped_builtin(int argc, char **argv)
{
	int	err;

	err = 0;
	if (!strcmp(argv[0], "echo"))
		err = echo_builtin(argc, argv);
	else if (!strcmp(argv[0], "env"))
		err = env_builtin();
	else if (!strcmp(argv[0], "pwd"))
		err = pwd_builtin();
	else
		printf("minishell: bug error: didn't recognize cmd\n");
	gc_clean(&(g_minishell.gcan));
	gc_clean(&(g_minishell.gcenv));
	exit(err);
}
