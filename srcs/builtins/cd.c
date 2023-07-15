/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatteux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 15:29:51 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/13 17:51:13 by tbatteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"
#include "minishell.h"
#include "libft.h"

void	update_env(char ***env)
{
	char	*buf;
	int		i;

	buf = malloc(1000);
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

void	cd(char *path, char **env)
{
	if (chdir(path) == -1)
		perror("error");
	update_env(&env);
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
