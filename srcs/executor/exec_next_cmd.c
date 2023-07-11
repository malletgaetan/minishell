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
	int		err;

	cmd->arg_len = count_words(*token);
	cmd->args = malloc(sizeof(char *) * (cmd->arg_len + 1));
	if (cmd->args == NULL)
		return (HARDFAIL_ERROR); // TODO hard fail, kill all childs, free everything, print error and exit
	if (pipe(cmd->pipeout))
	{
		free(cmd->args);
		return (HARDFAIL_ERROR); // TODO hard fail, kill all childs, free everything, print error and exit
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
			if (pipe(cmd->pipein))
				return (HARDFAIL_ERROR); // TODO hard fail, kill all childs, free everything, print error and exit
			if (fd_manual_pipe(STDIN_FILENO, cmd->pipein[1], (*token)->next->value))
				return (HARDFAIL_ERROR); // TODO, replace every hard fail error to HARDFAIL_ERR
			(*token) = (*token)->next;
		}
		else if ((*token)->type == S_REDIR_IN)
		{
			if (pipe(cmd->pipein))
				return (HARDFAIL_ERROR); // TODO hard fail, kill all childs, free everything, print error and exit
			(*token) = (*token)->next;
			err = file_to_pipe((*token)->value, cmd->pipein[1]);
			if (err != 0)
			{
				free(cmd->args);
				return (err); // TODO can be soft fail
			}
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
			return (HARDFAIL_ERROR);
		if (dup2(cmd->pipein[0], STDIN_FILENO) == -1)
			return (HARDFAIL_ERROR);
		if (close(cmd->pipein[0]))
			return (HARDFAIL_ERROR);
		if (pipereadfd != -1 && close(pipereadfd))
			return (HARDFAIL_ERROR);
	}
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
	if (close(cmd->pipeout[1])) // close write end of next pipe
		return (HARDFAIL_ERROR);
	if (cmd->pipein[0])
	{
		if (close(cmd->pipein[0]) || close(cmd->pipein[1]))
			return (HARDFAIL_ERROR);
	}
	if (pipereadfd != 0)  // close read end of prec pipe
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
	{
		if (close(cmd.pipeout[0]) || close(cmd.pipeout[1]) || close(cmd.pipein[0]) || close(cmd.pipein[1]))
			return (HARDFAIL_ERROR);
		return (exec_next_cmd(token->next, 0, pids, depth)); // for the moment next command after a failed command takes input from stdin, in this case it shouldn't pipe at all
	}
	pid = fork();
	if (pid == 0)
	{
		if (setup_child_pipes(&cmd, token == NULL, pipereadfd))
			return (HARDFAIL_ERROR);
		execve(cmd.args[0], cmd.args, NULL);
		free(cmd.args);
		return (errno);
	}
	if (pid == -1)
		return (HARDFAIL_ERROR);
	free(cmd.args);
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
	return (exec_next_cmd(token->next, cmd.pipeout[0], pids, depth + 1));
}
