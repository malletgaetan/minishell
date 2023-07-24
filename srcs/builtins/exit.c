/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 17:39:31 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/17 19:14:24 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_builtin(char **argv)
{
	int	i;

	kill_all_childs(SIGKILL);
	wait_all_childs();
	gc_clean(&(g_ms.gcan));
	gc_clean(&(g_ms.gcenv));
	i = 0;
	if (argv[1])
	{
		while (ft_isdigit(argv[1][i]))
			i++;
		if (!argv[1][i])
			exit(ft_atoi(argv[1]));
		else
			exit (1);
	}
	exit(0);
}
