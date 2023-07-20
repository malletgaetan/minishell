/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 17:19:03 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/17 18:19:06 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO maybe recode this as it doesn't seem correct
int	echo_builtin(int argc, char **argv)
{
	(void)argc;
	if (argv[1][0] == '-' && argv[1][1] == 'n')
	{
		ft_putstr_fd(argv[2], 1);
	}
	else
	{
		ft_putstr_fd(argv[1], 1);
		write(1, "\n", 1);
	}
	return (0);
}

/*
int	main(int argc, char **argv)
{
	echo (&argv[1]);
	return (0);
}
*/
