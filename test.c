#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int	main(void)
{
	int	fd;
	int	fds[2];

	fd = open("test.txt", O_RDONLY);
	if (pipe(fds))
		perror("error 1:");
	dup2(fds[0], STDOUT_FILENO);
	if (close(fds[0]))
		perror("error 3:");
	while (42)
	{
	}
}
