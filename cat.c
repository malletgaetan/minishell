# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>

int	main(int argc, char **argv)
{
	char	*line;

	printf("cat started %s\n", argv[1]);
	while ((line = readline("")))
	{
		printf("%s\n", line);
		free(line);
	}
}
