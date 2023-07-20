/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:37:36 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/17 18:14:34 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_add(char **env, char *arg)
{
	char	**new_env;
	int		i;

	new_env = gc_malloc(&(g_minishell.gcenv), (get_size(env) + 2) * sizeof(char *));
	if (new_env == NULL)
		return (NULL);
	i = -1;
	while (env[++i])
		new_env[i] = env[i];
	new_env[i] = gc_strdup(arg);
	new_env[i + 1] = 0;
	gc_free(&(g_minishell.gcenv), (void **)&env);
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

int	export_builtin(int argc, char **argv)
{
	int	i;
	char	**tmp;

	i = 1;
	while (i < argc)
	{
		if (verif(argv[i]) == 0)
		{
			tmp = ft_add(g_minishell.envs, argv[i]);
			if (tmp == NULL)
				return (1);
			g_minishell.envs = tmp;
		}
		i++;
	}
	return (0);
}

/*
int	main(int argc, char **argv, char **env)
{
	char	**new_env;
	int		i;

	new_env = get_env_copy(new_env, env);
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
