#include "minishell.h"

static char	**get_argv(t_cmd *cmd)
{
	size_t	i;
	t_token	*token;
	char	**argv;

	token = g_minishell.token;
	cmd->arg_len = get_nb_args(token) + 1;
	cmd->args = gc_malloc((&g_minishell.gcan), sizeof(char *) * cmd->arg_len);
	if (cmd->args == NULL)
		return (NULL);
	i = 0;
	while (token)
	{
		if (token->type == WORD)
			cmd->args[i++] = token->value;
		else
			token = token->next;
		if (token != NULL)
			token = token->next;
	}
	cmd->args[i] = NULL;
	return (cmd->args);
}

int	exec_simple_builtin(void)
{
	t_cmd	cmd;

	if (setup_cmd(&cmd))
		return (HARDFAIL_ERROR);
	if (setup_cmd(&cmd, &(g_minishell.token)))
		return (HARDFAIL_ERROR);
	if (!strcmp(cmd, "export"))
		return (export(cmd->arg_len, cmd->args, g_minishell.envs));
	if (!strcmp(cmd, "cd"))
		return (1);
	if (!strcmp(cmd, "unset"))
		return (1);
	if (!strcmp(cmd, "exit"))
		return (1);
	return (0);
}
