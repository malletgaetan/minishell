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
		hardfail_exit(errno);
	safe_pipe(cmd->pipeout);
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
			safe_pipe(cmd->pipein);
			fd_m_pipe(STDIN_FILENO, cmd->pipein[1], (*token)->next->value);
			(*token) = (*token)->next;
		}
		else if ((*token)->type == S_REDIR_IN)
		{
			safe_pipe(cmd->pipein);
			(*token) = (*token)->next;
			cmd->redirin_file = (*token)->value;
			err = file_to_pipe(cmd->redirin_file, cmd->pipein[1]);
			if (err != OK)
			{
				(*token) = (*token)->next;
				return (err);
			}
		}
		(*token) = (*token)->next;
	}
	if ((*token) && ((*token)->type == PIPE))
		(*token) = (*token)->next;
	cmd->args[arg_i] = NULL;
	return (OK);
}

static void	setup_child_pipes(t_cmd *cmd, int is_last_cmd, int *pipereadfd)
{
	if (is_opened_fd(cmd->pipein[0]))
	{
		safe_close(cmd->pipein + 1);
		safe_dup2(cmd->pipein[0], STDIN_FILENO);
		safe_close(cmd->pipein);
		safe_close(pipereadfd);
	}
	else if (is_opened_fd(*pipereadfd))
	{
		safe_dup2(*pipereadfd, STDIN_FILENO);
		safe_close(pipereadfd);
	}
	if (!is_last_cmd || (cmd->redirout_type != 0))
	{
		safe_close(cmd->pipeout);
		safe_dup2(cmd->pipeout[1], STDOUT_FILENO);
		safe_close(cmd->pipeout + 1);
	}
}

static void	unsetup_child_pipes(t_cmd *cmd, int *pipereadfd)
{
	safe_close(cmd->pipeout + 1);
	safe_close(cmd->pipein);
	safe_close(cmd->pipein + 1);
	safe_close(pipereadfd);
}

void	close_all_pipes(t_cmd *cmd, int *pipereadfd)
{
	safe_close(cmd->pipein);
	safe_close(cmd->pipein + 1);
	safe_close(cmd->pipeout);
	safe_close(cmd->pipeout + 1);
	safe_close(pipereadfd);
}

void	init_cmd(t_cmd *cmd)
{
	ft_memset(cmd, 0, sizeof(t_cmd));
	cmd->pipeout[0] = -1;
	cmd->pipeout[1] = -1;
	cmd->pipein[0] = -1;
	cmd->pipein[1] = -1;
}

void	exec_next_cmd(t_token *token, int pipereadfd, int depth)
{
	t_cmd	cmd;
	char	*path;
	int		err;

	if (token == NULL)
		return ;
	init_cmd(&cmd);
	err = setup_cmd(&cmd, &token);
	if (err == SOFTFAIL_ERROR)
	{
		close_all_pipes(&cmd, &pipereadfd);
		printf("minishell: %s: %s\n", strerror(errno), cmd.redirin_file);
		return (exec_next_cmd(token, 0, depth));
	}
	if (cmd.executable != NULL)
	{
		g_ms.pids[depth] = fork();
		if (g_ms.pids[depth] == 0)
		{
			setup_child_pipes(&cmd, token == NULL, &pipereadfd);
			if (is_builtin(cmd.executable))
				exec_builtin(cmd.arg_len, cmd.args);
			path = right_path(cmd.executable, g_ms.envs);
			execve(path, cmd.args, g_ms.envs);
			err = errno;
			if (err == ENOENT)
				printf("minishell: %s: command not found\n", cmd.executable);
			clean_exit(err);
		}
	}
	if (g_ms.pids[depth] == -1)
		hardfail_exit(errno);
	unsetup_child_pipes(&cmd, &pipereadfd);
	if (cmd.redirout_type != 0)
	{
		err = pipe_tofile(cmd.pipeout[0], cmd.redirout_file, cmd.redirout_type);
		safe_close(cmd.pipeout);
		cmd.pipeout[0] = 0;
		if (err == SOFTFAIL_ERROR)
		{
			kill(g_ms.pids[depth], SIGKILL);
			close_all_pipes(&cmd, &pipereadfd);
			printf("minishell: %s: %s\n", strerror(errno), cmd.redirout_file);
		}
	}
	if (token == NULL)
		return ;
	exec_next_cmd(token, cmd.pipeout[0], depth + 1);
}
