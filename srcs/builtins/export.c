/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatteux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:37:36 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/11 16:17:31 by tbatteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"
#include "minishell.h"
#include "libft.h"

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

int	verif(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=' && ((str[i - 1] >= 'A' && str[i - 1] <= 'Z')
		  		|| (str[i - 1] >= 'a' && str[i - 1] <= 'z')))
			return (0);
		i++;
	}
	return (1);
}
	//printf("invalid argument\n");
	// pas d'erreur ecrite par le vrai shell

char	**export(char **new_env, char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (verif(argv[i]) == 0)
			new_env = add(new_env, argv[i]);
		i++;
	}
	return (new_env);
}

/*
int	main(int argc, char **argv, char **env)
{
	char	**new_env;
	int		i;

	new_env = get_env(new_env, env);
	new_env = export(new_env, argv);
	i = 0;
	while (new_env[i])
		printf("%s\n", new_env[i++]);
	i = 0;
	while (new_env[i])
		free(new_env[i++]);
	free(new_env);
	return (0);
}*/
