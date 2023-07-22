/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 15:29:51 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/17 18:18:50 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/types.h>
#include <dirent.h>

void	ft_update_env(void)
{
	char	*buf;
	int		i;

	buf = ms_getenv("PWD");
	i = 0;
	while (g_ms.envs[i])
	{
		if (ft_strncmp(g_ms.envs[i], "PWD=", 4))
			break ;
		i++;
	}
	g_ms.envs[i] = buf;
}

int	cd_builtin(int argc, char **argv)
{
	int	fd;

	if (argc == 1)
		return (0);
	if (opendir(argv[1]) == NULL)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == -1)
		{
			print_error("cd", strerror(errno), argv[1]);
			close(fd);
			return (1);
		}
		close(fd);
	}
	if (chdir(argv[1]) == -1)
	{
		print_error("cd", strerror(errno), argv[1]);
		return (1);
	}
	ft_update_env();
	return (0);
}
