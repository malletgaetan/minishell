/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:16:02 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/17 18:11:45 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(void)
{
	int	i;

	i = 0;
	while (g_minishell.envs[i])
		printf("%s\n", g_minishell.envs[i++]);
	return (0);
}
