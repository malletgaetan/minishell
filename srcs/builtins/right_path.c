/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   right_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 18:17:29 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/17 17:42:44 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_path(char **env)
{
	int	x;

	x = 0;
	while (env[x])
	{
		if (ft_strncmp(env[x], "PATH", 4) == 0)
			break ;
		x++;
	}
	return (&env[x][5]);
}

void	free_split(char **tab)
{
	int	i;

	i = -1;
	while (tab[++i])
		free(tab[i]);
	free(tab);
}

char	*right_path(char *cmd, char **env)
{
	char	**tab_path;
	char	*path;
	char	*newenv;
	int		x;

	if (*cmd == '/' || *cmd == '.')
		return (cmd);
	newenv = find_path(env);
	tab_path = ft_split_path(newenv, ':');
	x = -1;
	while (tab_path[++x])
	{
		path = ft_strjoin(tab_path[x], cmd);
		if (access(path, X_OK) == 0)
		{
			free_split(tab_path);
			return (path);
		}
		free(path);
	}
	free_split(tab_path);
	return (NULL);
}
