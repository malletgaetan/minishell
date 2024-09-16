/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 10:53:01 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/17 18:32:06 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd_builtin(int argc, char **argv)
{
	char	buf[PATH_MAX];

	(void)argv;
	if (argc == 2)
		return (print_error("pwd", "too many arguments", NULL));
	getcwd(buf, sizeof(buf));
	printf("%s\n", buf);
	return (0);
}
