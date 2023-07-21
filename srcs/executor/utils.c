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
	if (g_minishell.pids == NULL)
		return ;
	while (g_minishell.pids[i] != 0)
		waitpid(g_minishell.pids[i++], NULL, 0);
}

void	kill_all_childs(int sig, uint32_t start)
{
	uint32_t	i;

	if (g_minishell.pids == NULL)
		return ;
	i = start;
	while (g_minishell.pids[i] != 0)
		kill(g_minishell.pids[i++], sig);
}

int	safe_close(int *fd)
{
	if (*fd == -1)
		return (OK);
	if (close(*fd))
		return (HARDFAIL_ERROR);
	*fd = -1;
	return (OK);
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

