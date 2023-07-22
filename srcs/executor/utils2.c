/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatteux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 16:42:09 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/20 16:42:28 by tbatteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!strcmp(cmd, "echo"))
		return (1);
	if (!strcmp(cmd, "env"))
		return (1);
	if (!strcmp(cmd, "pwd"))
		return (1);
	return (is_unpiped_builtin(cmd));
}

int	is_opened_fd(int fd)
{
	return (fd != -1);
}

void	safe_close(int *fd)
{
	if (*fd == -1)
		return ;
	if (close(*fd))
		hardfail_exit(errno);
	*fd = -1;
}

void	safe_pipe(int pids[2])
{
	if (pipe(pids) == -1)
		hardfail_exit(errno);
}


void	safe_dup2(int oldfd, int newfd)
{
	if (dup2(oldfd, newfd) == -1)
		hardfail_exit(errno);
}
