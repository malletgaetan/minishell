/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:17:24 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/17 19:12:28 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_env(char **env)
{
	int	i;

	g_minishell.envs = gc_malloc(&(g_minishell.gcenv), (get_size(env) + 1) * sizeof(char **));
	if (g_minishell.envs == NULL)
		return (1);
	i = -1;
	while (env[++i])
		g_minishell.envs[i] = gc_strdup(env[i]);
	g_minishell.envs[i] = NULL;
	return (0);
}
