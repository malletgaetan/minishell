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

	new_env = gc_malloc(&(g_ms.gcenv), (get_size(env) + 2) * sizeof(char *));
	if (new_env == NULL)
		return (NULL);
	i = -1;
	while (env[++i])
		new_env[i] = env[i];
	new_env[i] = gc_strdup(arg);
	new_env[i + 1] = 0;
	gc_free(&(g_ms.gcenv), (void **)&env);
	return (new_env);
}

int	verif(char *str)
{
	int	i;

	i = 0;
	while (ft_isenv(str[i]))
		i++;
	return (str[i] == '=');
}

char	*name(char *str)
{
	int		i;
	int		j;
	char	*name;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	name = malloc((i + 1) * sizeof(char));
	if (!name)
		return (NULL);
	j = -1;
	while (++j < i)
		name[j] = str[j];
	name[j] = '\0';
	return (name);
}

int	read_env(char **env, char *n_var)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(n_var, env[i], ft_strlen(n_var)) == 0)
			return (i);
		i++;
	}
	return (-1);
}

int	export_builtin(int argc, char **argv)
{
	int		i;
	int		j;
	char	*n_var;
	char	**tmp;

	i = 0;
	while (++i < argc)
	{
		n_var = name(argv[i]);
		j = read_env(g_ms.envs, n_var);
		if (j >= 0 && (verif(argv[i]) == 1))
		{
			gc_free(&(g_ms.gcenv), (void **)&(g_ms.envs[j]));
			g_ms.envs[j] = gc_strdup(argv[i]);
		}		
		else if (verif(argv[i]) == 1)
		{
			tmp = ft_add(g_ms.envs, argv[i]);
			if (tmp == NULL)
				return (1);
			g_ms.envs = tmp;
		}
		free(n_var);
	}
	return (0);
}
