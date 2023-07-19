#include "minishell.h"

static int	setup_cmd(t_cmd *cmd, t_token **token)
{
	int		arg_i;
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
			if (err != OK)
				return (err);
		}
		(*token) = (*token)->next;
	}
	if ((*token) && ((*token)->type == PIPE))
		(*token) = (*token)->next;
	cmd->args[arg_i] = NULL;
	return (OK);
}

static int	setup_child_pipes(t_cmd *cmd, int is_last_cmd, int *pipereadfd)
{
	// attach stdin to read end of file pipe
	if (cmd->pipein[0] != 0)
	{
		if (close_zero(cmd->pipein + 1))
			return (HARDFAIL_ERROR);
		if (dup2(cmd->pipein[0], STDIN_FILENO) == -1)
			return (HARDFAIL_ERROR);
		if (close_zero(cmd->pipein))
			return (HARDFAIL_ERROR);
		if (close_zero(pipereadfd))
			return (HARDFAIL_ERROR);
	}
	// attach stdin to read end of old cmd pipe
	else if (pipereadfd != 0)
	{
		if (dup2(*pipereadfd, STDIN_FILENO) == -1)
			return (HARDFAIL_ERROR);
		if (close_zero(pipereadfd))
			return (HARDFAIL_ERROR);
	}
	// attach stdout to end of pipe
	if (!is_last_cmd || (cmd->redirout_type != 0))
	{
		if (close_zero(cmd->pipeout))
			return (HARDFAIL_ERROR);
		if (dup2(cmd->pipeout[1], STDOUT_FILENO) == -1)
			return (HARDFAIL_ERROR);
		if (close_zero(cmd->pipeout + 1))
			return (HARDFAIL_ERROR);
	}
	return (OK);
}

static int	unsetup_child_pipes(t_cmd *cmd, int *pipereadfd)
{
	// close write end of next pipe
	if (close_zero(cmd->pipeout + 1))
		return (HARDFAIL_ERROR);
	if (cmd->pipein[0])
	{
		if (close_zero(cmd->pipein) || close_zero(cmd->pipein + 1))
			return (HARDFAIL_ERROR);
	}
	// close read end of prec pipe
	return (close_zero(pipereadfd));
}

int	close_all_pipes(t_cmd *cmd, int *pipereadfd)
{
	if (close_zero(cmd->pipein))
		return (HARDFAIL_ERROR);
	if (close_zero(cmd->pipein + 1))
		return (HARDFAIL_ERROR);
	if (close_zero(cmd->pipeout))
		return (HARDFAIL_ERROR);
	if (close_zero(cmd->pipeout + 1))
		return (HARDFAIL_ERROR);
	return (close_zero(pipereadfd));
}

int	exec_next_cmd(t_token *token, int pipereadfd, int depth)
{
	t_cmd	cmd;
	char	*path;
	int		err;
	
	if (token == NULL)
		return (OK);
	ft_memset(&cmd, 0, sizeof(t_cmd));
	err = setup_cmd(&cmd, &token);
	if (err == HARDFAIL_ERROR)
		return (err);
	// ignore (cd, export, exit or unset) in pipes
	if (err == SOFTFAIL_ERROR || is_unpiped_builtin(cmd.args[0]))
	{
		if (close_all_pipes(&cmd, &pipereadfd))
			return (HARDFAIL_ERROR);
		if (err == SOFTFAIL_ERROR)
			printf("minishell: software error: %s\n", strerror(errno)); // TODO have correct message
		return (exec_next_cmd(token, 0, depth));
	}
	g_minishell.pids[depth] = fork();
	if (g_minishell.pids[depth] == 0)
	{
		if (setup_child_pipes(&cmd, token == NULL, &pipereadfd))
			return (errno);
		path = right_path(cmd.args[0], g_minishell.envs);
		execve(path, cmd.args, NULL);
		return (errno);
	}
	if (g_minishell.pids[depth] == -1)
		return (HARDFAIL_ERROR);
	if (unsetup_child_pipes(&cmd, &pipereadfd))
		return (HARDFAIL_ERROR);
	if (cmd.redirout_type != 0)
	{
		err = pipe_to_file(cmd.pipeout[0], cmd.redirout_file, cmd.redirout_type);
		if (close_zero(cmd.pipeout))
			return (HARDFAIL_ERROR);
		cmd.pipeout[0] = 0;
		if (err == HARDFAIL_ERROR)
			return (err);
		if (err == SOFTFAIL_ERROR)
		{
			kill(g_minishell.pids[depth], SIGKILL);
			if (close_all_pipes(&cmd, &pipereadfd))
				return (HARDFAIL_ERROR);
			printf("minishell: software error: %s\n", strerror(errno)); // TODO have correct message
			--depth; // TODO fix this ugly sht, made bc don't wait to increment depth if command didn't executed properly
		}
	}
	if (token == NULL)
		return (OK);
	return (exec_next_cmd(token, cmd.pipeout[0], depth + 1));
}
