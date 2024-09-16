/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmallet <gmallet@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 16:28:06 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/22 08:36:12 by gmallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_minishell	g_ms;

int	event(void)
{
	return (0);
}

static int	init_minishell(char **env)
{
	g_ms.bad_token = NULL;
	g_ms.token = NULL;
	g_ms.old_status = 0;
	gc_init(&(g_ms.gcan));
	gc_init(&(g_ms.gcenv));
	setup_sigactions();
	if (get_env_copy(env))
		return (1);
	rl_event_hook = event;
	return (0);
}

int	print_error(char *msg_a, char *msg_b, char *msg_c)
{
	ft_putstr_fd(msg_a, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(msg_b, STDERR_FILENO);
	if (msg_c == NULL)
	{
		ft_putstr_fd("\n", STDERR_FILENO);
		return (1);
	}
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(msg_c, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	return (1);
}

int	main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	signal(SIGQUIT, SIG_IGN);
	if (init_minishell(env))
	{
		print_error("minishell", "internal error", strerror(errno));
		return (1);
	}
	interpret_loop();
	gc_clean(&(g_ms.gcan));
	gc_clean(&(g_ms.gcenv));
	return (g_ms.old_status);
}
