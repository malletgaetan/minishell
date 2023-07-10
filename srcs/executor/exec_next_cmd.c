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

static int	setup_cmd(t_cmd *cmd, t_token **token)
{
	size_t	arg_i;

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
			fd_manual_pipe(STDIN_FILENO, cmd->pipein[1], (*token)->next->value);
			(*token) = (*token)->next;
		}
		else if ((*token)->type == S_REDIR_IN)
		{
			pipe(cmd->pipein);
			(*token) = (*token)->next;
			file_to_pipe((*token)->value, cmd->pipein[1]); // TODO check error
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
	else if (pipereadfd != 0)
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
	if (pipereadfd != 0)  // close read end of prec pipe
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
	ft_memset(&cmd, 0, sizeof(t_cmd));
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
		pipe_to_file(cmd.pipeout[0], cmd.redirout_file, cmd.redirout_type); // TODO check error
		close(cmd.pipeout[0]);
		cmd.pipeout[0] = 0;
	}
	pids[depth] = pid;
	if (token == NULL)
		return (0);
	return (exec_next_cmd(token->next, cmd.pipeout[0], pids, depth + 1));
}
