/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 15:29:51 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/17 18:18:50 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_update_env(void)
{
	char	*buf;
	int		i;

	buf = ms_getenv("PWD");
	printf("ms_getenv fine\n");
	i = 0;
	while (g_minishell.envs[i])
	{
		if (ft_strncmp(g_minishell.envs[i], "PWD=", 4))
			break ;
		i++;
	}
	// gc_free(&(g_minishell.gcenv), (void **)&(g_minishell.envs[i])); nasty fix
	g_minishell.envs[i] = buf;
}

int	cd_builtin(int argc, char **argv)
{
	if (argc == 1)
		return (0);
	if (chdir(argv[1]) == -1)
		return (-1);
	ft_update_env();
	return (0);
}

