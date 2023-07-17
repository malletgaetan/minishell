/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 16:23:30 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/17 18:15:14 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**suppr(char **env, char *argv)
{
	char **new_env;
	int		i;
	int		j;

	new_env = malloc(get_size((env) + 1) * sizeof(char *));
	i = 0;
	j = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], argv, ft_strlen(argv)) != 0)
			new_env[j++] = env[i];
		i++;
	}
	new_env[i] = 0;
	free(env);
	return (new_env);
}

char	**ft_unset(char **new_env, char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
		new_env = suppr(new_env, argv[i++]);
	return (new_env);
}

/*
int	main(int argc, char **argv, char **env)
{
	char	**new_env;
	int		i;

	new_env = get_env(new_env, env);
	new_env = unset(new_env, argv);
	i = 0;
	while (new_env[i])
		printf("%s\n", new_env[i++]);
	i = 0;
	while (new_env[i])
		free(new_env[i++]);
	free(new_env);
	return (0);
}*/
