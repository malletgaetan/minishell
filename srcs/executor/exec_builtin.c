/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_piped_builtin.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatteux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 16:38:17 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/20 16:38:18 by tbatteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_builtin(int argc, char **argv)
{
	int	err;

	err = 0;
	if (!strcmp(argv[0], "echo"))
		err = echo_builtin(argc, argv);
	else if (!strcmp(argv[0], "cd"))
		err = cd_builtin(argc, argv);
	else if (!strcmp(argv[0], "env"))
		err = env_builtin();
	else if (!strcmp(argv[0], "export"))
		err = export_builtin(argc, argv);
	else if (!strcmp(argv[0], "unset"))
		err = unset_builtin(argc, argv);
	else if (!strcmp(argv[0], "exit"))
		exit_builtin();
	else if (!strcmp(argv[0], "pwd"))
		err = pwd_builtin();
	else
		printf("minishell: bug error: didn't recognize cmd\n");
	gc_clean(&(g_minishell.gcan));
	gc_clean(&(g_minishell.gcenv));
	if (err)
		exit(errno);
	exit(0);
}
