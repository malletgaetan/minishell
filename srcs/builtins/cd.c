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
	int	ret;
	
	if (argc == 1)
		return (0);
	if (opendir(argv[1]) == NULL)
	{
		ret = open(argv[1], O_RDONLY);
		if (ret != -1)
			printf("cd: not a directory: %s\n", argv[1]);
		else
			printf("cd: no such file or directory: %s\n", argv[1]);
		close(ret);
	}
	if (chdir(argv[1]) == -1)
		return (-1);
	ft_update_env();
	return (0);
}
