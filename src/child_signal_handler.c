#include <sys/wait.h>
#include <stddef.h>

void sig_handler(int s)
{
	int status;
	wait4(-1, &status, WNOHANG, NULL);
}
