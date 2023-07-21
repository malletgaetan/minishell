/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_unpiped_builtin.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatteux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 16:38:27 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/20 16:38:28 by tbatteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	setup_cmd(t_cmd *cmd, t_token *token)
{
	size_t	arg_i;

	cmd->arg_len = get_nb_args(token);
	cmd->args = gc_malloc((&g_ms.gcan), sizeof(char *) * (cmd->arg_len + 1));
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

int	exec_unpiped_builtin(void)
{
	t_cmd	cmd;

	cmd.arg_len = get_nb_args(g_ms.token);
	if (setup_cmd(&cmd, g_ms.token))
		return (HARDFAIL_ERROR);
	if (!strcmp(cmd.args[0], "export"))
		return (export_builtin(cmd.arg_len, cmd.args));
	else if (!strcmp(cmd.args[0], "cd"))
		return (cd_builtin(cmd.arg_len, cmd.args));
	else if (!strcmp(cmd.args[0], "unset"))
		return (unset_builtin(cmd.arg_len, cmd.args));
	else if (!strcmp(cmd.args[0], "exit"))
		exit_builtin();
	else
		printf("minishell: bug error: didn't recognize cmd\n");
	return (0);
}
