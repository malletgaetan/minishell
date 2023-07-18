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

void	get_env(char **env)
{
	int	i;

	new_env = gc_malloc(&(g_minishell.gcan), (get_size(env) + 1) * sizeof(char *));
	i = -1;
	while (env[++i])
		g_minishell.envs[i] = ft_strdup(env[i]);// rajouter les strdup a gcan
	g_minishell.envs[i] = 0;
}
