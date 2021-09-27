#define SERVER_PORT 56765
#define BUFFER_SIZE 2048

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include "erproc.h"
#include "str_search_ptrn.h"
#include "accept_key.h"

char *response = "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: ";
char *socket_request = "Sec-WebSocket-Key:";
char *hello_message = "Hello from c-socket!";

int main(void)
{
	int opt = 1;
	char buf[BUFFER_SIZE];
	char request_key[24];
	char response_key[MESSAGE_SIZE];
	struct sockaddr_in addr;
	int response_len = strlen(response);

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
		Write(1, buf, ssize);

/*		Write(sockfd, response, strlen(response)); */

		int pos = str_search_ptrn(socket_request, buf, ssize);

		for(int i = 0; i < 24; ++i) {
			request_key[i] = buf[pos+19+i];
		};

		Write(1, request_key, 24);
		printf("\n");

		accept_key_generator(request_key, response_key);
		Write(1, response_key, MESSAGE_SIZE);
		printf("\n");
		
		Write(sockfd, response, response_len);
		Write(sockfd, response_key, MESSAGE_SIZE);
		Write(sockfd, "\r\n\r\n", 4);

		sleep(1);
		
		Write(sockfd, hello_message, 20);

		/* Close(sockfd); */
	}

	return 0;
}
