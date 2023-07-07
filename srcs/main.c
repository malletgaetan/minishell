#include "minishell.h"

t_minishell	g_minishell;

size_t	count_words(t_token *token)
{
	size_t	c;

	c = 0;
	while (token && token->type != PIPE)
	{
		if (token == WORD)
		{
			token = token->next;
			++c;
			continue ;
		}
		token = token->next;
		if (token)
			token = token->next;
	}
	return (c);
}

// TODO: add support for parenthesis
int	ex_next_cmd(t_token *token, int pipereadfd, int pids[10], int depth)
{
	t_cmd	cmd;
	int	pid;
	size_t	arg_i;
	
	if (token == NULL)
		return (1);
	cmd.arg_len = count_words(token);
	cmd.args = malloc(sizeof(char *) * (cmd.arg_len + 1));
	if (pipe(cmd.fds))
		return (2);
	if (cmd.args == NULL)
		return (MALLOC_ERROR);
	arg_i = 0;
	while (token && (token->type != PIPE))
	{
		if (token->type == WORD)
			cmd.args[arg_i++] = token->value;
		token = token->next;
	}
	cmd.args[arg_i] = NULL;
	// TODO if << read from stdin and redirect to pipe stdin process
	pid = fork();
	if (pid == 0)
	{
		// child
		// attach stdin to start of pipe (if fdin != 0)
		if (pipereadfd != 0)
		{
			// printf("edit piperead %d\n", depth);
			dup2(pipereadfd, STDIN_FILENO);
			close(pipereadfd);
		}
		// attach stdout to end of pipe (if token != NULL)
		if (token != NULL)
		{
			// printf("edit stdout %d\n", depth);
			dup2(cmd.fds[1], STDOUT_FILENO);
			close(cmd.fds[1]);
		}
		execve(cmd.args[0], cmd.args, NULL);
	}
	// close(pipereadfd);
	// parent
	pids[depth] = pid;
	if (token == NULL)
		return (0);
	return (ex_next_cmd(token->next, cmd.fds[0], pids, depth + 1));
}

int	ex_cmds(void)
{
	int	pids[10] = {0};
	t_token	*token;
	int	err;
	int	i;

	token = g_minishell.token;
	err = ex_next_cmd(token, 0, pids, 0);
	if (err != 0)
	{
		perror("pipe error:");
		printf("error ! Oo %d\n", err);
	}
	i = 0;
	while (i < 2)
	{
		printf("wait %d\n", i);
		wait(NULL);
		++i;
	}
	printf("end of it\n");
	// while ((i < 10) && pids[i] != 0)
	//	wait(pids[i++]);
	return (0);
}

int	main(int argc, char **argv)
{
	int	err;

	if (argc == 1)
		return (0);
	if ((err = lex(argv[1])))
	{
		printf("got err: %d while lexing\n", err);
		return (1);
	}
	if (ex_cmds())
	{
		printf("got error while executing cmds\n");
	}
	return (0);
}
