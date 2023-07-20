#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <stdio.h>


void	sigint(int code)
{
	(void)code;
	printf("child received sigint\n");
	exit(0);
}

void	sigquit(int code)
{
	(void)code;
	printf("child received sigquit\n");
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
	struct sigaction	sa;
	struct sigaction	sb;

	setup_sigaction(&sa, SIGINT, SA_RESTART, sigint);
	setup_sigaction(&sb, SIGQUIT, SA_RESTART, sigquit);
}

int	main(void)
{
	setup_sigactions();
	while (1)
	{
	}
}
