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
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], "PWD=", 4))
			break ;
		i++;
	}
	free((*env)[i]);
	(*env)[i] = buf;
}


int	cd_builtin(int argc, char **argv)
{
	if (chdir(path) == -1)
		return (-1);
	ft_update_env(&env);
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
