#include "minishell.h"

static int	setup_cmd(t_cmd *cmd, t_token *token)
{
	size_t	arg_i;
	int		err;

	cmd->arg_len = get_nb_args(token);
	cmd->args = gc_malloc((&g_minishell.gcan), sizeof(char *) * (cmd->arg_len + 1));
	if (cmd->args == NULL)
		return (HARDFAIL_ERROR);
	arg_i = 0;
	while (token != NULL)
	{
		if (token->type == WORD)
			cmd->args[arg_i++] = token->value;
		token = token->next;
	}
	cmd->args[arg_i] = NULL;
	return (OK);
}

// nb_cmds == 1 && is in (export, cd, unset, exit) => exec_simple_builtin

int	exec_nonpiped_builtin(void)
{
	t_cmd	cmd;

	cmd.arg_len = get_nb_args(g_minishell.token);
	if (setup_cmd(&cmd, g_minishell.token))
		return (HARDFAIL_ERROR);
	if (strcmp(cmd.args[0], "export")) // could use a trie? TODO could this crash if malloc fail? -> to handle
		return (export_builtin(cmd.arg_len, cmd.args));
	else if (strcmp(cmd.args[0], "cd"))
		return (cd_builtin(cmd.arg_len, cmd.args));
	else if (strcmp(cmd.args[0], "unset"))
		return (unset_builtin(cmd.arg_len, cmd.args));
	else
		return (exit_builtin(cmd.arg_len, cmd.args));
}