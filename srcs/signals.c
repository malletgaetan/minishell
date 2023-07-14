#include "minishell.h"

void	sigint(int code)
{
	(void)code;
	g_minishell.sigint = 1;
}

void	sigquit(int code)
{
	(void)code;
}

void	sigterm(int code)
{
	(void)code;
}

void	setup_sigaction(struct sigaction *sa, int sig, int flags, void (*h)(int))
{
	sa->sa_handler = h;
	sa->sa_flags = flags;
    sigemptyset(&(sa->sa_mask));
    sigaction(sig, sa, 0);
}

void	setup_sigactions(void)
{
	setup_sigaction(&(g_minishell.sa_int), SIGINT, SA_RESTART, sigint);
	setup_sigaction(&(g_minishell.sa_quit), SIGQUIT, SA_RESTART, sigint);
	setup_sigaction(&(g_minishell.sa_term), SIGTERM, SA_RESTART, sigint);
}

