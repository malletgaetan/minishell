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
// int	ex_next_cmd(t_token *token, int oldpipe[2], int pids[10], int depth)
// {
// 	t_cmd	cmd;
// 	int	pid;
// 	size_t	arg_i;
	
// 	if (token == NULL)
// 		return (1);
// 	cmd.arg_len = count_words(token);
// 	cmd.args = malloc(sizeof(char *) * (cmd.arg_len + 1));
// 	if ((depth == 0) && pipe(cmd.fds))
// 		return (2);
// 	if (cmd.args == NULL)
// 		return (MALLOC_ERROR);
// 	arg_i = 0;
// 	while (token && (token->type != PIPE))
// 	{
// 		if (token->type == WORD)
// 			cmd.args[arg_i++] = token->value;
// 		token = token->next;
// 	}
// 	cmd.args[arg_i] = NULL;
// 	// TODO if << read from stdin and redirect to pipe stdin process
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		// child
// 		// attach stdin to start of pipe (if fdin != 0)
// 		if (pipereadfd != -1)
// 		{
// 			close(oldpipe[1]);
// 			printf("->> dup2 %d | fork %d\n", dup2(oldpipe[0], STDIN_FILENO), depth);
// 			printf("closing %d | fork %d\n", oldpipe[0], depth);
// 			close(oldpipe[0]);
// 		}
// 		// attach stdout to end of pipe (if token != NULL)
// 		if (token != NULL)
// 		{
// 			close(cmd.fds[0]);
// 			dup2(cmd.fds[1], STDOUT_FILENO);
// 			close(cmd.fds[1]);
// 		}
// 		execve(cmd.args[0], cmd.args, NULL);
// 	}
// 	// printf("closing %d\n", cmd.fds[1]);
// 	// close(cmd.fds[1]); // close write end of next pipe
// 	// printf("closing %d\n", pipereadfd);
// 	// close(pipereadfd); // close read end of prec pipe
// 	// (cat)[1] -> [0](ls)
// 	// parent
// 	pids[depth] = pid;
// 	if (token == NULL)
// 		return (0);
// 	return (ex_next_cmd(token->next, cmd.fds, pids, depth + 1));
// }


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
		if (pipereadfd != -1)
		{
			dup2(pipereadfd, STDIN_FILENO);
			close(pipereadfd);
		}
		// attach stdout to end of pipe (if token != NULL)
		if (token != NULL)
		{
			close(cmd.fds[0]);
			dup2(cmd.fds[1], STDOUT_FILENO);
		}
		execve(cmd.args[0], cmd.args, NULL);
	}
	close(cmd.fds[1]); // close write end of next pipe
	if (pipereadfd != -1)
		close(pipereadfd); // close read end of prec pipe
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
	err = ex_next_cmd(token, -1, pids, 0);
	if (err != 0)
		perror("pipe error:");
	i = 0;
	while ((i < 10) && (pids[i] != 0))
		waitpid(pids[i++], NULL, 0);
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
