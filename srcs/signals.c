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
	if (g_minishell.status == STATUS_IDLE)
	{
		ft_putstr_fd("\nminishell$>", STDERR_FILENO);
		return ;
	}
	g_minishell.sigint = 1;
}

void	sigquit(int code)
{
	(void)code;
	ft_putstr_fd("\b\b  \b\b", STDERR_FILENO);
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
	setup_sigaction(&(g_minishell.sa_int), SIGINT, SA_RESTART, sigint);
	setup_sigaction(&(g_minishell.sa_quit), SIGQUIT, SA_RESTART, sigquit);
}
