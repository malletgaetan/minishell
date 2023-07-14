/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 17:39:31 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/14 18:09:16 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_all_childs(t_minishell *minishell)
{
	int	i;
	
	i = 0;
	while (minishell->pids[i])
		waitpid(minishell->pids[i++], NULL);
}

void	clean_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		free(env[i++]);
	free(env);
}

void	ft_exit(char **env)
{
	clean_env(env);
	kill_all_childs(SIGKILL, 0);
	wait_all_childs();
	gc_clean(&(g_minishell.gcan));
	exit(0);
}
