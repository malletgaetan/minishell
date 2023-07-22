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

void	exec_cmds(void)
{
	uint32_t	i;
	int			killed;

	g_ms.pids = gc_malloc(&(g_ms.gcan), sizeof(int) * (g_ms.nb_cmds + 1));
	if (g_ms.pids == NULL)
		hardfail_exit(errno);
	ft_memset(g_ms.pids, 0, sizeof(int) * (g_ms.nb_cmds + 1));
	exec_next_cmd(g_ms.token, -1, 0);
	i = 0;
	killed = 0;
	setup_sigaction(&(g_ms.sa_int), SIGINT, 0, sigint); // NO SA_RESTART
	while (i < g_ms.nb_cmds)
	{
		if (g_ms.pids[i] == 0)
		{
			++i;
			continue ;
		}
		if (!killed && g_ms.sigint != 0)
		{
			setup_sigaction(&(g_ms.sa_int), SIGINT, SA_RESTART, sigint);
			kill_all_childs(SIGINT, i);
			wait_all_childs();
		}
		if (waitpid(g_ms.pids[i], &(g_ms.old_status), 0) == -1)
		{
			if (errno == EINTR)
			{
				setup_sigaction(&(g_ms.sa_int), SIGINT, SA_RESTART, sigint);
				kill_all_childs(SIGINT, i);
				wait_all_childs();
				break ;
			}
			setup_sigaction(&(g_ms.sa_int), SIGINT, SA_RESTART, sigint);
			hardfail_exit(errno);
		}
		g_ms.pids[i] = 0;
		++i;
	}
	if (g_ms.sa_int.sa_flags == 0)
		setup_sigaction(&(g_ms.sa_int), SIGINT, SA_RESTART, sigint);
	gc_free(&(g_ms.gcan), (void **)&(g_ms.pids));
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

void	exec(void)
{
	char	*cmd;

	cmd = next_cmd_name();
	if (is_unpiped_builtin(cmd))
	{
		exec_unpiped_builtin();
		return ;
	}
	exec_cmds();
}
