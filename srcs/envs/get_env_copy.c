/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_copy.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:17:24 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/20 12:27:26 by tbatteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*new_shlvl(char *str, int nb)
{
	char	*final;
	char	*n;
	int		i;
	int		j;

	n = ft_itoa(nb);
	final = gc_malloc(&(g_ms.gcenv), ft_strlen(str) + ft_strlen(n));
	i = -1;
	while (str[++i])
		final[i] = str[i];
	j = 0;
	while (n[j])
		final[i++] = n[j++];
	final[i] = '\0';
	free(n);
	return (final);
}

int	get_env_copy(char **env)
{
	int	i;
	int	nb;

	g_ms.envs = gc_malloc(&(g_ms.gcenv), (get_size(env) + 1) * sizeof(char **));
	if (g_ms.envs == NULL)
		return (1);
	i = -1;
	while (env[++i])
	{
		if (strncmp(env[i], "SHLVL=", 6) == 0)
		{
			nb = ft_atoi(&env[i][6]);
			nb++;
			g_ms.envs[i] = new_shlvl("SHLVL=", nb);
		}
		else
			g_ms.envs[i] = gc_strdup(env[i]);
	}
	g_ms.envs[i] = NULL;
	return (0);
}
