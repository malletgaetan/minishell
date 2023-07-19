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

void	ft_update_env(void)
{
	char	*buf;
	int		i;

	buf = getenv("PWD");
	i = 0;
	while (g_minishell.envs[i])
	{
		if (ft_strncmp(g_minishell.envs[i], "PWD=", 4))
			break ;
		i++;
	}
	free((*env)[i]); // free un seul maillon ?
	g_minishell.envs[i] = buf;
}


int	cd_builtin(char **argv)
{
	if (chdir(argv[1]) == -1)
		return (-1);
	ft_update_env();
}

/*
int	main(int argc, char **argv, char **env)
{
	char	buf[1000];

	printf("%s\n", getcwd(buf, 1000));
	cd(argv[1], env);
	printf("%s\n", getcwd(buf, 1000));
	return (0);
}
*/
