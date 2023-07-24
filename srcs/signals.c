/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatteux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 16:30:48 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/20 16:30:49 by tbatteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint(int code)
{
	(void)code;
	g_ms.sigint = 1;
	if (g_ms.status == STATUS_IDLE)
	{
		rl_done = 1;
		return ;
	}
}

void	setup_sigaction(t_sigaction *sa, int sig, int flags, void (*h)(int))
{
	sa->sa_handler = h;
	sa->sa_flags = flags;
	sigemptyset(&(sa->sa_mask));
	sigaction(sig, sa, 0);
}

void	setup_sigactions(void)
{
	setup_sigaction(&(g_ms.sa_int), SIGINT, SA_RESTART, sigint);
}
