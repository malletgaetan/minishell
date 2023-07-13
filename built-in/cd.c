/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatteux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 15:29:51 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/13 16:20:11 by tbatteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	cd(char *path)
{
	if (chdir(path) == -1)
		perror("error");
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
