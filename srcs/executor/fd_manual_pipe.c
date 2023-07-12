#include "minishell.h"

int	fd_manual_pipe(int fdfrom, int fdto, char *delim)
{
	ssize_t	ret;
	
	ret = read(fdfrom, g_minishell.buf, BUF_SIZE);
	while (ret != 0)
	{
		if (ret < 0)
			return (HARDFAIL_ERROR);
		if (delim != NULL)
		{
			if (ft_strncmp(g_minishell.buf, delim, (size_t)ret))
				return (0);
		}
		if (write(fdto, g_minishell.buf, ret) == -1)
			return (HARDFAIL_ERROR);
		ret = read(fdfrom, g_minishell.buf, BUF_SIZE);
	}
	return (0);
}

int	pipe_to_file(int fdfrom, char *fileto, int redirtype)
{
	int	fd;
	int	err;
	int	flags;

	flags = O_CREAT | O_WRONLY;
	if (redirtype == D_REDIR_OUT)
		flags |= O_APPEND;
	fd = open(fileto, flags);
	if (fd == -1)
	{
		if (errno == EACCES)
			return (SOFTFAIL_ERROR);
		return (HARDFAIL_ERROR);
	}
	err = fd_manual_pipe(fdfrom, fd, NULL);
	close(fd);
	return (err);
}

int	file_to_pipe(char *filefrom, int fdto)
{
	int	fd;
	int	err;

	fd = open(filefrom, O_RDONLY);
	if (fd == -1)
	{
		if (errno == EACCES)
			return (SOFTFAIL_ERROR);
		if (errno == ENOENT)
			return (SOFTFAIL_ERROR);
		return (HARDFAIL_ERROR);
	}
	err = fd_manual_pipe(fd, fdto, NULL);
	close(fd);
	return (err);
}
