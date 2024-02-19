/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_next_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 09:35:13 by gmallet           #+#    #+#             */
/*   Updated: 2023/07/22 09:35:14 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	word(t_cmd *cmd, t_token **token, int *i)
{
	if (cmd->executable == NULL)
		cmd->executable = (*token)->value;
	cmd->args[(*i)++] = (*token)->value;
}

static void	redirout(t_cmd *cmd, t_token **token)
{
	cmd->redirout_type = (*token)->type;
	(*token) = (*token)->next;
	cmd->redirout_file = (*token)->value;
}

static int d_redirin(t_cmd *cmd, t_token **token)
{
	safe_pipe(cmd->pipein);
	if (fd_m_pipe(STDIN_FILENO, cmd->pipein[1], (*token)->next->value))
		return (INT_ERROR);
	(*token) = (*token)->next;
	return (0);
}

static int	s_redirin(t_cmd *cmd, t_token **token)
{
	int	err;

	safe_pipe(cmd->pipein);
	(*token) = (*token)->next;
	cmd->redirin_file = (*token)->value;
	err = file_to_pipe(cmd->redirin_file, cmd->pipein[1]);
	if (err != OK)
	{
		(*token) = (*token)->next;
		return (err);
	}
	return (OK);
}

int	setup_next_cmd(t_cmd *cmd, t_token **token)
{
	int		arg_i;

	cmd->arg_len = get_nb_args(*token);
	cmd->args = gc_malloc((&g_ms.gcan), sizeof(char *) * (cmd->arg_len + 1));
	if (cmd->args == NULL)
		hardfail_exit(errno);
	safe_pipe(cmd->pipeout);
	arg_i = 0;
	while ((*token) && ((*token)->type != PIPE))
	{
		if ((*token)->type == WORD)
			word(cmd, token, &arg_i);
		else if (((*token)->type == D_REDIR_OUT)
			|| ((*token)->type == S_REDIR_OUT))
			redirout(cmd, token);
		else if ((*token)->type == D_REDIR_IN)
		{
			if (d_redirin(cmd, token))
				return (INT_ERROR);
		}
		else if ((*token)->type == S_REDIR_IN && s_redirin(cmd, token))
			return (SOFTFAIL_ERROR);
		(*token) = (*token)->next;
	}
	if ((*token) && ((*token)->type == PIPE))
		(*token) = (*token)->next;
	cmd->args[arg_i] = NULL;
	return (OK);
}
