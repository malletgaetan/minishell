/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 10:53:01 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/15 09:06:37 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	pwd_builtin(void)
{
	char	*buf;
	int		x;

	x = 4096; // max len of combined path name + file name is 4096 bytes
	buf = malloc(x);
	getcwd(buf, x);
	printf("%s\n", buf);
	free(buf);
}

/*
int	main(void)
{
	pwd();
	return (0);
}
*/
