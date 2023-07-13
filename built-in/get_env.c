/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatteux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:17:24 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/10 16:05:07 by tbatteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

char	**get_env(char **new_env, char **env)
{
	int	i;

	new_env = malloc((get_taille(env) + 1) * sizeof(char *));
	i = -1;
	while (env[++i])
		new_env[i] = ft_strdup(env[i]);
	new_env[i] = 0;
	return (new_env);
}
