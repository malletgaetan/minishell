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
int	ex_next_cmd(int fdin)
{
	t_cmd	cmd;
	int		pid;
	t_token	*token;
	size_t	arg_i;
	// char	*line;

	token = g_minishell.token;
	cmd.arg_len = count_words(token);
	if (pipe(cmd.fds))
		return (1);
	cmd.args = malloc(sizeof(char *) * (cmd.arg_len + 1));
	if (cmd.args == NULL)
		return (MALLOC_ERROR);
	arg_i = 0;
	while (token && (token->type != PIPE))
	{
		if (token->type == WORD)
		{
			cmd.args[arg_i++] = token->value;
		}
		// else
		// {
		// 	if (token->type == S_REDIR_OUT)
		// 	{
		// 		;
		// 	}
		// 	else if (token->type == D_REDIR_OUT)
		// 	{
		// 		;
		// 	}
		// 	else if (token->type == S_REDIR_IN)
		// 	{
		// 		;
		// 	}
		// 	else if (token->type == D_REDIR_IN)
		// 	{
		// 		;
		// 	}
		// }
		token = token->next;
	}
	cmd.args[arg_i] = NULL;
	// TODO if << read from stdin and redirect to pipe stdin process
	// fork()
	pid = fork();
	if (pid == 0)
	{
		// child
		// close(cmd.fds[1]);
		// dup2(cmd.fds[0], STDIN_FILENO);
		// close(cmd.fds[0]);
		execve(cmd.args[0], cmd.args, NULL);
	}
	(void)fdin;
	// close(cmd.fds[0]);
	// dup2(cmd.fds[1], fdin);
	return (pid);
	// recursive call ex_next_cmd
}

int	ex_cmds(void)
{
	int	pid;

	pid = ex_next_cmd(STDIN_FILENO);
	waitpid(pid, NULL, 0);
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
