/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatteux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 10:53:01 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/13 11:41:40 by tbatteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"
#include "minishell.h"
#include "libft.h"

void	pwd(void)
{
	char	*buf;
	int		x;

	x = 1000;
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
