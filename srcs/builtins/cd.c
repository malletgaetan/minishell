/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 15:29:51 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/17 18:18:50 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/types.h>
#include <dirent.h>

int	cd_builtin(int argc, char **argv)
{
	char	*dir;

	if (argc > 2)
		return (print_error("cd", "missing $HOME", NULL));

	if (argc == 1) {
		dir = ms_getenv("HOME");
		if (dir == NULL)
			return (print_error("cd", "missing $HOME", NULL));
	} else {
		dir = argv[1];
	}

	if (chdir(dir) < 0)
		return (print_error("cd", strerror(errno), argv[1]));
	return (0);
}
