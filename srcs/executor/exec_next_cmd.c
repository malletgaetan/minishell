/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_next_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatteux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 16:38:10 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/20 16:38:11 by tbatteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	setup_cmd(t_cmd *cmd, t_token **token)
{
	int		arg_i;
	int		err;

	cmd->arg_len = get_nb_args(*token);
	cmd->args = gc_malloc((&g_ms.gcan), sizeof(char *) * (cmd->arg_len + 1));
	if (cmd->args == NULL)
		return (HARDFAIL_ERROR);
	if (pipe(cmd->pipeout))
		return (HARDFAIL_ERROR);
	arg_i = 0;
	while ((*token) && ((*token)->type != PIPE))
	{
		if ((*token)->type == WORD)
		{
			if (cmd->executable == NULL)
					cmd->executable = (*token)->value;
			cmd->args[arg_i++] = (*token)->value;
		}
		else if (((*token)->type == D_REDIR_OUT)
			|| ((*token)->type == S_REDIR_OUT))
		{
			cmd->redirout_type = (*token)->type;
			(*token) = (*token)->next;
			cmd->redirout_file = (*token)->value;
		}
		else if ((*token)->type == D_REDIR_IN)
		{
			if (pipe(cmd->pipein))
				return (HARDFAIL_ERROR);
			if (fd_m_pipe(STDIN_FILENO, cmd->pipein[1], (*token)->next->value))
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
	if (is_opened_fd(cmd->pipein[0]))
	{
		if (safe_close(cmd->pipein + 1))
			return (HARDFAIL_ERROR);
		if (dup2(cmd->pipein[0], STDIN_FILENO) == -1)
			return (HARDFAIL_ERROR);
		if (safe_close(cmd->pipein))
			return (HARDFAIL_ERROR);
		if (safe_close(pipereadfd))
			return (HARDFAIL_ERROR);
	}
	// attach stdin to read end of old cmd pipe
	else if (is_opened_fd(*pipereadfd))
	{
		if (dup2(*pipereadfd, STDIN_FILENO) == -1)
			return (HARDFAIL_ERROR);
		if (safe_close(pipereadfd))
			return (HARDFAIL_ERROR);
	}
	// attach stdout to end of pipe
	if (!is_last_cmd || (cmd->redirout_type != 0))
	{
		if (safe_close(cmd->pipeout))
			return (HARDFAIL_ERROR);
		if (dup2(cmd->pipeout[1], STDOUT_FILENO) == -1)
			return (HARDFAIL_ERROR);
		if (safe_close(cmd->pipeout + 1))
			return (HARDFAIL_ERROR);
	}
	return (OK);
}

static int	unsetup_child_pipes(t_cmd *cmd, int *pipereadfd)
{
	// close write end of next pipe
	if (safe_close(cmd->pipeout + 1))
		return (HARDFAIL_ERROR);
	if (cmd->pipein[0])
	{
		if (safe_close(cmd->pipein) || safe_close(cmd->pipein + 1))
			return (HARDFAIL_ERROR);
	}
	// close read end of prec pipe
	return (safe_close(pipereadfd));
}

int	close_all_pipes(t_cmd *cmd, int *pipereadfd)
{
	if (safe_close(cmd->pipein))
		return (HARDFAIL_ERROR);
	if (safe_close(cmd->pipein + 1))
		return (HARDFAIL_ERROR);
	if (safe_close(cmd->pipeout))
		return (HARDFAIL_ERROR);
	if (safe_close(cmd->pipeout + 1))
		return (HARDFAIL_ERROR);
	return (safe_close(pipereadfd));
}

void	init_cmd(t_cmd *cmd)
{
	ft_memset(cmd, 0, sizeof(t_cmd));
	cmd->pipeout[0] = -1;
	cmd->pipeout[1] = -1;
	cmd->pipein[0] = -1;
	cmd->pipein[1] = -1;
}

int	exec_next_cmd(t_token *token, int pipereadfd, int depth)
{
	t_cmd	cmd;
	char	*path;
	int		err;

	if (token == NULL)
		return (OK);
	init_cmd(&cmd);
	err = setup_cmd(&cmd, &token);
	if (err == HARDFAIL_ERROR)
		return (err);
	if (err == SOFTFAIL_ERROR)
	{
		if (close_all_pipes(&cmd, &pipereadfd))
			return (HARDFAIL_ERROR);
		if (err == SOFTFAIL_ERROR)
			printf("minishell: %s: %s\n", strerror(errno), cmd.redirout_file);
		return (exec_next_cmd(token, 0, depth));
	}
	if (cmd.executable != NULL)
	{
		g_ms.pids[depth] = fork();
		if (g_ms.pids[depth] == 0)
		{
			if (setup_child_pipes(&cmd, token == NULL, &pipereadfd))
				return (errno); // TODO handle error in waitpid
			if (is_builtin(cmd.args[0]))
				exec_builtin(cmd.arg_len, cmd.args);
			path = right_path(cmd.args[0], g_ms.envs);
			execve(path, cmd.args, g_ms.envs);
			return (errno); // TODO handle error in waitpid
		}
	}
	if (g_ms.pids[depth] == -1)
		return (HARDFAIL_ERROR);
	if (unsetup_child_pipes(&cmd, &pipereadfd))
		return (HARDFAIL_ERROR);
	if (cmd.redirout_type != 0)
	{
		err = pipe_tofile(cmd.pipeout[0], cmd.redirout_file, cmd.redirout_type);
		if (safe_close(cmd.pipeout))
			return (HARDFAIL_ERROR);
		cmd.pipeout[0] = 0;
		if (err == HARDFAIL_ERROR)
			return (err);
		if (err == SOFTFAIL_ERROR)
		{
			kill(g_ms.pids[depth], SIGKILL);
			if (close_all_pipes(&cmd, &pipereadfd))
				return (HARDFAIL_ERROR);
			printf("minishell: %s: %s\n", strerror(errno), cmd.redirout_file);
		}
	}
	if (token == NULL)
		return (OK);
	return (exec_next_cmd(token, cmd.pipeout[0], depth + 1));
}
