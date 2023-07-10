/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatteux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:37:36 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/10 16:04:49 by tbatteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	get_taille(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}

char	**add(char **env, char *arg)
{
	char	**new_env;
	int		i;

	new_env = malloc((get_taille(env) + 2) * sizeof(char *));
	i = -1;
	while (env[++i])
		new_env[i] = env[i];
	new_env[i] = ft_strdup(arg);
	new_env[i + 1] = 0;
	free(env);
	return (new_env);
}

char	**export(char **new_env, char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
		new_env = add(new_env, argv[i++]);
	return (new_env);
}

// verifier les args si ils sont coherent

int	main(int argc, char **argv, char **env)
{
	char	**new_env;
	int		i;

	new_env = get_env(new_env, env);
	new_env = export(new_env, argv);
	i = 0;
	while (new_env[i])
		printf("%s\n", new_env[i++]);
	return (0);
}
