#define SERVER_PORT 56765

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "erproc.h"

int main(void)
{
	int opt = 1;
	char buf[2048];
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERVER_PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int ls = Socket(AF_INET, SOCK_STREAM, 0);
	
	Setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	Bind(ls, (struct sockaddr *) &addr, sizeof(addr));

	Listen(ls, 16);
	printf("Server started on port %d.\n", SERVER_PORT);

	while(1) {
		int sockfd = Accept(ls, NULL, NULL);

		ssize_t ssize = Read(sockfd, buf, 2048);
		write(1, buf, ssize);
	}

	return 0;
}
