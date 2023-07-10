#include "minishell.h"

static size_t	count_words(t_token *token)
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

static int	pipefrom(int fdfrom, int fdto, t_token *token)
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

static int	writefrom(int writefd, char *filename, int redirtype)
{
	int	fd;
	int	opt;
	ssize_t	ret;

	opt = O_CREAT | O_WRONLY;
	if (redirtype == D_REDIR_OUT)
		opt |= O_APPEND;
	fd = open(filename, opt, 0644);
	if (fd == -1)
		perror("open:");
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

static int	setup_cmd(t_cmd *cmd, t_token **token)
{
	size_t	arg_i;
	int		fd;

	cmd->arg_len = count_words(*token);
	cmd->args = malloc(sizeof(char *) * (cmd->arg_len + 1));
	if (cmd->args == NULL)
		return (MALLOC_ERROR);
	if (pipe(cmd->pipeout))
	{
		free(cmd->args);
		return (PIPE_ERROR);
	}
	arg_i = 0;
	while ((*token) && ((*token)->type != PIPE))
	{
		if ((*token)->type == WORD)
			cmd->args[arg_i++] = (*token)->value;
		else if (((*token)->type == D_REDIR_OUT) || ((*token)->type == S_REDIR_OUT))
		{
			cmd->redirout_type = (*token)->type;
			(*token) = (*token)->next;
			cmd->redirout_file = (*token)->value;
		}
		else if ((*token)->type == D_REDIR_IN)
		{
			pipe(cmd->pipein);
			pipefrom(STDIN_FILENO, cmd->pipein[1], (*token)->next);
			(*token) = (*token)->next;
		}
		else if ((*token)->type == S_REDIR_IN)
		{
			pipe(cmd->pipein);
			fd = open((*token)->next->value, O_RDONLY);
			pipefrom(fd, cmd->pipein[1], (*token)->next);
			(*token) = (*token)->next;
			close(fd);
		}
		(*token) = (*token)->next;
	}
	cmd->args[arg_i] = NULL;
	return (0);
}

static int	setup_child_pipes(t_cmd *cmd, int is_last_cmd, int pipereadfd)
{
	if (cmd->pipein[0] != 0)
	{
		if (close(cmd->pipein[1]))
			return (OS_ERROR);
		if (dup2(cmd->pipein[0], STDIN_FILENO) == -1)
			return (OS_ERROR);
		if (close(cmd->pipein[0]))
			return (OS_ERROR);
		if (pipereadfd != -1 && close(pipereadfd))
			return (OS_ERROR);
	}
	else if (pipereadfd != -1)
	{
		if (dup2(pipereadfd, STDIN_FILENO) == -1)
			return (OS_ERROR);
		if (close(pipereadfd))
			return (OS_ERROR);
	}
	// attach stdout to end of pipe (if token != NULL)
	if (!is_last_cmd || (cmd->redirout_type != 0))
	{
		if (close(cmd->pipeout[0]))
			return (OS_ERROR);
		if (dup2(cmd->pipeout[1], STDOUT_FILENO) == -1)
			return (OS_ERROR);
		if (close(cmd->pipeout[1]))
			return (OS_ERROR);
	}
	return (0);
}


static int	unsetup_child_pipes(t_cmd *cmd, int pipereadfd)
{
	if (close(cmd->pipeout[1])) // close write end of next pipe
		return (OS_ERROR);
	if (cmd->pipein[0])
	{
		if (close(cmd->pipein[0]) || close(cmd->pipein[1]))
			return (OS_ERROR);
	}
	if (pipereadfd != -1)  // close read end of prec pipe
	{
		if (close(pipereadfd))
			return (OS_ERROR);
	}
	return (0);
}

int	exec_next_cmd(t_token *token, int pipereadfd, int pids[10], int depth)
{
	t_cmd	cmd;
	int		pid;
	
	if (token == NULL)
		return (0);
	cmd.pipein[0] = 0;
	cmd.pipein[1] = 0;
	cmd.pipeout[0] = 0;
	cmd.pipeout[1] = 0;
	cmd.redirout_type = 0;
	setup_cmd(&cmd, &token);
	pid = fork();
	if (pid == 0)
	{
		if (setup_child_pipes(&cmd, token == NULL, pipereadfd))
			return (OS_ERROR);
		execve(cmd.args[0], cmd.args, NULL);
	}
	free(cmd.args);
	if (unsetup_child_pipes(&cmd, pipereadfd))
		return (OS_ERROR);
	if (cmd.redirout_type != 0)
	{
		writefrom(cmd.pipeout[0], cmd.redirout_file, cmd.redirout_type);
		close(cmd.pipeout[0]);
		cmd.pipeout[0] = -1;
	}
	pids[depth] = pid;
	if (token == NULL)
		return (0);
	return (exec_next_cmd(token->next, cmd.pipeout[0], pids, depth + 1));
}
