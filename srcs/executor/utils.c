/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatteux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 16:38:43 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/20 16:38:45 by tbatteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_all_childs(void)
{
	uint32_t	i;

	i = 0;
	if (g_ms.pids == NULL)
		return ;
	while (i < g_ms.nb_cmds)
	{
		if (g_ms.pids[i] == 0)
		{
			++i;
			continue ;
		}
		waitpid(g_ms.pids[i++], &(g_ms.old_status), 0);
	}
}

void	kill_all_childs(int sig, uint32_t start)
{
	uint32_t	i;

	if (g_ms.pids == NULL)
		return ;
	i = start;
	while (g_ms.pids[i] != 0)
	{
		if (g_ms.pids[i] == 0)
		{
			++i;
			continue ;
		}
		kill(g_ms.pids[i++], sig);
	}

}

size_t	get_nb_args(t_token *token)
{
	int	c;

	c = 0;
	while (token && token->type != PIPE)
	{
		if (token->type == WORD)
		{
			token = token->next;
			++c;
			continue ;
		}
		token = token->next;
		if (token)
			token = token->next;
	}
	return (c);
}

int	is_unpiped_builtin(char *cmd)
{
	if (cmd == NULL)
		return (0);
	if (!strcmp(cmd, "cd"))
		return (1);
	if (!strcmp(cmd, "export"))
		return (1);
	if (!strcmp(cmd, "unset"))
		return (1);
	if (!strcmp(cmd, "exit"))
		return (1);
	return (0);
}
