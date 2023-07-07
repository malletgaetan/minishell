# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>

int	main(int argc, char **argv)
{
	char	*line;
	
	while ((line = readline("")))
	{
		printf("%s\n", line);
		free(line);
	}
}
