/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatteux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 17:19:03 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/13 17:38:35 by tbatteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

// prends les options et l'argument a ecrire dans un char **

void	ft_putstr(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		write(1, &str[i++], 1);
}

void	echo(char **arg)
{

	if (arg[0][0] == '-' && arg[0][1] == 'n')
		ft_putstr(arg[1]);
	else
	{
		ft_putstr(arg[0]);
		write(1, "\n", 1);
	}
}

/*
int	main(int argc, char **argv)
{
	echo (&argv[1]);
	return (0);
}
*/
