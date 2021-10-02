#include <sys/wait.h>
#include <stddef.h>

void sig_handler(int s)
{
	signal(SIGCHLD, sig_handler);
	int status;
	wait4(-1, &status, WNOHANG, NULL);
}
