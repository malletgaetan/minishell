/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_manual_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatteux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 16:38:36 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/20 16:38:38 by tbatteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	delim_cmp(char *buf, char *delim)
{
	while (*delim != '\0')
	{
		if (*buf == '\n')
			return (1);
		if (*(buf++) != *(delim++))
			return (1);
	}
	return (0);
}

// should use readline in case of readfile and writing to pipe
// -> if line is longer than BUF_SIZE, this will not act as intended
void	fd_m_pipe(int fdfrom, int fdto, char *delim)
{
	ssize_t	ret;

	ret = read(fdfrom, g_ms.buf, BUF_SIZE);
	while (ret != 0)
	{
		if (ret < 0)
			hardfail_exit(errno);
		if (delim != NULL)
		{
			if (!delim_cmp(g_ms.buf, delim))
				return ;
		}
		if (write(fdto, g_ms.buf, ret) == -1)
			hardfail_exit(errno);
		ret = read(fdfrom, g_ms.buf, BUF_SIZE);
	}
}

int	pipe_tofile(int fdfrom, char *fileto, int redirtype)
{
	int	fd;
	int	flags;

	flags = O_CREAT | O_WRONLY;
	if (redirtype == D_REDIR_OUT)
		flags |= O_APPEND;
	fd = open(fileto, flags, 0666);
	if (fd == -1)
	{
		if (errno == EACCES)
			return (SOFTFAIL_ERROR);
		hardfail_exit(errno);
	}
	fd_m_pipe(fdfrom, fd, NULL);
	safe_close(&fd);
	return (OK);
}

int	file_to_pipe(char *filefrom, int fdto)
{
	int	fd;

	fd = open(filefrom, O_RDONLY);
	if (fd == -1)
	{
		if (errno == EACCES)
			return (SOFTFAIL_ERROR);
		if (errno == ENOENT)
			return (SOFTFAIL_ERROR);
		hardfail_exit(errno);
	}
	fd_m_pipe(fd, fdto, NULL);
	safe_close(&fd);
	return (OK);
}
