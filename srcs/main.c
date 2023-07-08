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

int	pipefrom(int fdfrom, int fdto, t_token *token)
{
	ssize_t		ret;
	
	if (token->type != WORD)
		return (1);
	ret = read(fdfrom, g_minishell.buf, BUF_SIZE);
	while (ret != 0)
	{
		if (ret < 0)
			return (ret);
		write(fdto, g_minishell.buf, ret);
		ret = read(fdfrom, g_minishell.buf, BUF_SIZE);
	}
	return (0);
}

int	writefrom(int writefd, char *filename, int redirtype)
{
	int	fd;
	int	opt;
	ssize_t	ret;

	opt = O_CREAT | O_WRONLY;
	if (redirtype == D_REDIR_OUT)
		opt |= O_APPEND;
	fd = open(filename, opt, 0644);
	if (fd == -1)
	{
		perror("open:");
	}
	ret = read(writefd, g_minishell.buf, BUF_SIZE);
	while (ret != 0)
	{
		if (ret < 0)
		{
			close(fd);
			return (ret);
		}
		write(fd, g_minishell.buf, ret);
		ret = read(writefd, g_minishell.buf, BUF_SIZE);
	}
	close(fd);
	return (0);
}


int	ex_next_cmd(t_token *token, int pipereadfd, int pids[10], int depth)
{
	t_cmd	cmd;
	int	pipein[2] = {0};
	int	pid;
	int	redir_type = 0;
	char	*redir_file;
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
		else if ((token->type == D_REDIR_OUT) || (token->type == S_REDIR_OUT))
		{
			redir_type = token->type;
			token = token->next;
			redir_file = token->value;
		}
		else if (token->type == D_REDIR_IN)
		{
			pipe(pipein);
			pipefrom(STDIN_FILENO, pipein[1], token->next);
			token = token->next;
		}
		else if (token->type == S_REDIR_IN)
		{
			pipe(pipein);
			int	filed;
			filed = open(token->next->value, O_RDONLY);
			pipefrom(filed, pipein[1], token->next);
			token = token->next;
			close(filed);
		}
		token = token->next;
	}
	cmd.args[arg_i] = NULL;
	// TODO if << read from stdin and redirect to pipe stdin process
	pid = fork();
	if (pid == 0)
	{
		// child
		// attach stdin to start of pipe (if fdin != 0)
		if (pipein[0] != 0)
		{
			close(pipein[1]);
			dup2(pipein[0], STDIN_FILENO);
			close(pipein[0]);
			if (pipereadfd != -1)
				close(pipereadfd);
		}
		else if (pipereadfd != -1)
		{
			dup2(pipereadfd, STDIN_FILENO);
			close(pipereadfd);
		}
		// attach stdout to end of pipe (if token != NULL)
		if ((token != NULL) || (redir_type != 0))
		{
			close(cmd.fds[0]);
			dup2(cmd.fds[1], STDOUT_FILENO);
			close(cmd.fds[1]);
		}
		execve(cmd.args[0], cmd.args, NULL);
	}
	close(cmd.fds[1]); // close write end of next pipe
	if (pipein[0])
	{
		close(pipein[0]);
		close(pipein[1]);
	}
	if (pipereadfd != -1)
		close(pipereadfd); // close read end of prec pipe
	if (redir_type != 0)
	{
		writefrom(cmd.fds[0], redir_file, redir_type);
		close(cmd.fds[0]);
		cmd.fds[0] = -1;
	}
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
