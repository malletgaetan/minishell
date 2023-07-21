/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatteux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 16:38:02 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/20 16:38:04 by tbatteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_cmds(void)
{
	int			err;
	uint32_t	i;

	g_ms.pids = gc_malloc(&(g_ms.gcan), sizeof(int) * (g_ms.nb_cmds + 1));
	if (g_ms.pids == NULL)
		return (HARDFAIL_ERROR);
	ft_memset(g_ms.pids, 0, sizeof(int) * (g_ms.nb_cmds + 1));
	err = exec_next_cmd(g_ms.token, -1, 0);
	if (err == HARDFAIL_ERROR)
		kill_all_childs(SIGKILL, 0);
	i = 0;
	setup_sigaction(&(g_ms.sa_int), SIGINT, 0, sigint); // NO SA_RESTART
	while (g_ms.pids[i] != 0)
	{
		if (g_ms.sigint != 0)
		{
			setup_sigaction(&(g_ms.sa_int), SIGINT, SA_RESTART, sigint);
			kill_all_childs(SIGKILL, i);
		}
		if (waitpid(g_ms.pids[i], NULL, 0) == -1)
		{
			setup_sigaction(&(g_ms.sa_int), SIGINT, SA_RESTART, sigint);
			kill_all_childs(SIGKILL, i);
			continue ;
		}
		++i;
	}
	if (g_ms.sa_int.sa_flags == 0)
		setup_sigaction(&(g_ms.sa_int), SIGINT, SA_RESTART, sigint);
	gc_free(&(g_ms.gcan), (void **)&(g_ms.pids));
	return (err);
}

char	*next_cmd_name(void)
{
	t_token	*token;

	token = g_ms.token;
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
	if (is_unpiped_builtin(cmd))
		return (exec_unpiped_builtin());
	return (exec_cmds());
}
