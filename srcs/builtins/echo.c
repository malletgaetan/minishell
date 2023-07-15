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

#include "gc.h"
#include "minishell.h"
#include "libft.h"

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
