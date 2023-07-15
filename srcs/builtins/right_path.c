/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   right_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatteux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 18:17:29 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/13 18:22:26 by tbatteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"
#include "minishell.h"
#include "libft.h"

char	*find_path(char **env)
{
	int	x;

	x = 0;
	while ((*env)[x])
	{
		if (ft_strncmp(env[x], "PATH", 4) == 0)
			break ;
		x++;
	}
	return (&env[x][5]);
}

char	*right_path(char *cmd, char **env)
{
	char	**tab_path;
	char	*path;
	char	*newenv;
	int		x;

	newenv = find_path(env);
	tab_path = ft_split(newenv, ':');
	x = 0;
	while (tab_path[x])
	{
		path = ft_strjoin(tab_path[x], cmd);
		if (access(path, X_OK) == 0)
			break ;
		free(path);
		x++;
	}
	x = -1;
	while (tab_path[++x])
		free(tab_path[x]);
	free(tab_path);
	return (path);
}
