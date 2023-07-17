/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 10:53:01 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/17 18:27:25 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd_builtin(void)
{
	char	*buf;

	buf = gc_malloc(&(g_minishell.gcan), sizeof(char) * 4096);
	getcwd(buf, 4096);
	ft_printf("%s\n", buf);
	gc_free(&(g_minishell.gcan), (void **)&buf);
}

/*
int	main(void)
{
	pwd();
	return (0);
}
*/
