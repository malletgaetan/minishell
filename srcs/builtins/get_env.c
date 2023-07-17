/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:17:24 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/17 18:20:27 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_env()
{
	int	i;
	char	**new_env;

	new_env = malloc((get_size(env) + 1) * sizeof(char *));
	i = -1;
	while (env[++i])
		new_env[i] = ft_strdup(env[i]);
	new_env[i] = 0;
	return (new_env);
}
