#include <sys/wait.h>
#include <stddef.h>

void sig_handler(int s)
{
	int status;
	while(wait4(-1, &status, WNOHANG, NULL) > 0)
		;
}
