#include "minishell.h"

static size_t	get_nb_args(t_token *token)
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
	int		err;

	cmd->arg_len = get_nb_args(*token);
	cmd->args = gc_malloc((&g_minishell.gcan), sizeof(char *) * (cmd->arg_len + 1));
	if (cmd->args == NULL)
		return (HARDFAIL_ERROR);
	if (pipe(cmd->pipeout))
		return (HARDFAIL_ERROR);
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
			if (pipe(cmd->pipein))
				return (HARDFAIL_ERROR);
			if (fd_manual_pipe(STDIN_FILENO, cmd->pipein[1], (*token)->next->value))
				return (HARDFAIL_ERROR);
			(*token) = (*token)->next;
		}
		else if ((*token)->type == S_REDIR_IN)
		{
			if (pipe(cmd->pipein))
				return (HARDFAIL_ERROR);
			(*token) = (*token)->next;
			err = file_to_pipe((*token)->value, cmd->pipein[1]);
			if (err == SOFTFAIL_ERROR)
			{
				; // TODO handle
			}
			else if (err == HARDFAIL_ERROR)
				return (err);
		}
		(*token) = (*token)->next;
	}
	cmd->args[arg_i] = NULL;
	return (0);
}

static int	setup_child_pipes(t_cmd *cmd, int is_last_cmd, int pipereadfd)
{
	// attach stdin to read end of file pipe
	if (cmd->pipein[0] != 0)
	{
		if (close(cmd->pipein[1]))
			return (HARDFAIL_ERROR);
		if (dup2(cmd->pipein[0], STDIN_FILENO) == -1)
			return (HARDFAIL_ERROR);
		if (close(cmd->pipein[0]))
			return (HARDFAIL_ERROR);
		if (pipereadfd != 0 && close(pipereadfd))
			return (HARDFAIL_ERROR);
	}
	// attach stdin to read end of old cmd pipe
	else if (pipereadfd != 0)
	{
		if (dup2(pipereadfd, STDIN_FILENO) == -1)
			return (HARDFAIL_ERROR);
		if (close(pipereadfd))
			return (HARDFAIL_ERROR);
	}
	// attach stdout to end of pipe
	if (!is_last_cmd || (cmd->redirout_type != 0))
	{
		if (close(cmd->pipeout[0]))
			return (HARDFAIL_ERROR);
		if (dup2(cmd->pipeout[1], STDOUT_FILENO) == -1)
			return (HARDFAIL_ERROR);
		if (close(cmd->pipeout[1]))
			return (HARDFAIL_ERROR);
	}
	return (0);
}

static int	unsetup_child_pipes(t_cmd *cmd, int pipereadfd)
{
	// close write end of next pipe
	if (close(cmd->pipeout[1]))
		return (HARDFAIL_ERROR);
	if (cmd->pipein[0])
	{
		if (close(cmd->pipein[0]) || close(cmd->pipein[1]))
			return (HARDFAIL_ERROR);
	}
	// close read end of prec pipe
	if (pipereadfd != 0)
	{
		if (close(pipereadfd))
			return (HARDFAIL_ERROR);
	}
	return (0);
}

int	exec_next_cmd(t_token *token, int pipereadfd, int pids[10], int depth)
{
	t_cmd	cmd;
	int		err;
	int		pid;
	
	if (token == NULL)
		return (0);
	ft_memset(&cmd, 0, sizeof(t_cmd));
	err = setup_cmd(&cmd, &token);
	if (err == HARDFAIL_ERROR)
		return (err);
	if (err == SOFTFAIL_ERROR)
		return (HARDFAIL_ERROR); // TODO handle softfail
	pid = fork();
	if (pid == 0)
	{
		if (setup_child_pipes(&cmd, token == NULL, pipereadfd))
			return (HARDFAIL_ERROR);
		execve(cmd.args[0], cmd.args, NULL);
		return (errno);
	}
	if (pid == -1)
		return (HARDFAIL_ERROR);
	pids[depth] = pid;
	if (unsetup_child_pipes(&cmd, pipereadfd))
		return (HARDFAIL_ERROR);
	if (cmd.redirout_type != 0)
	{
		err = pipe_to_file(cmd.pipeout[0], cmd.redirout_file, cmd.redirout_type);
		if (close(cmd.pipeout[0]))
			return (HARDFAIL_ERROR);
		if (err != 0)
			kill(pid, SIGKILL);
		cmd.pipeout[0] = 0;
	}
	if (token == NULL)
		return (0);
	return (exec_next_cmd(token->next, cmd.pipeout[0], pids, depth + 1));
}
