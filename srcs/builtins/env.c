/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:16:02 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/17 19:14:31 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_builtin(void)
{
	int	i;

	i = 0;
	while (g_minishell.envs[i])
		printf("%s\n", g_minishell.envs[i++]);
	return (0);
}
