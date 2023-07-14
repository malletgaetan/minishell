#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

struct sigaction sa;

void sig_int(int signum)
{
	printf("got signal %d\n", signum);
}

int main()
{
    int status,ret;
	int	pid;

    sa.sa_handler = sig_int;
	sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, 0);
    pid = fork();
    if(pid == 0)
	{
		sa.sa_flags = 0;
        while(1)
		{
            sleep(10);
        }
    }
	sa.sa_flags = 0;
    sigaction(SIGINT, &sa, 0);
	ret = waitpid(pid, &status, 0);
	printf("ca quitte interrupt %s\n", strerror(errno));
	return 0;
}
