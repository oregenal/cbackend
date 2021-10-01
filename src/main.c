#define SERVER_PORT 56765
#define BUFFER_SIZE 2048
#define REQUEST_KEY_SIZE 24

#define FIN_AND_MASK 0b10000000
#define TEXT_OPCODE 0x1
#define CLOSE_OPCODE 0x8
#define PAYLOAD_LEN_MASK 0b01111111

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "erproc.h"
#include "str_search_ptrn.h"
#include "accept_key.h"
#include "child_signal_handler.h"

char *response = "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: ";
char *socket_request = "Sec-WebSocket-Key:";


int main(void)
{
	int opt = 1;
	char buf[BUFFER_SIZE];
	char request_key[REQUEST_KEY_SIZE];
	char response_key[MESSAGE_SIZE];
	struct sockaddr_in addr;
	int response_len = strlen(response);
	char payload_mask[4];
	char message[BUFFER_SIZE];

	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERVER_PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	signal(SIGCHLD, sig_handler);

	int ls = Socket(AF_INET, SOCK_STREAM, 0);
	
	Setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	Bind(ls, (struct sockaddr *) &addr, sizeof(addr));

	Listen(ls, 16);
	printf("Server started on port %d.\n", SERVER_PORT);

	for(;;) {
		int sockfd = Accept(ls, NULL, NULL);

		int pid = fork();
		if(pid) {
			Close(sockfd);
			continue;
		}

		ssize_t ssize = Read(sockfd, buf, BUFFER_SIZE);

		int pos = str_search_ptrn(socket_request, buf, ssize);

		for(int i = 0; i < REQUEST_KEY_SIZE; ++i) {
			request_key[i] = buf[pos+19+i];
		};

		accept_key_generator(request_key, response_key);
		
		Write(sockfd, response, response_len);
		Write(sockfd, response_key, MESSAGE_SIZE);
		Write(sockfd, "\r\n\r\n", 4);

		for(;;) {
			ssize = Read(sockfd, buf, BUFFER_SIZE);

			uint8_t decode = buf[0];
			if((decode & CLOSE_OPCODE) == CLOSE_OPCODE) {
				printf("Close connection.\n");
				exit(EXIT_SUCCESS);
			};
			if((decode & TEXT_OPCODE) != TEXT_OPCODE) {
				printf("Non text payload.\n");
				continue;
			};
			if(decode && FIN_AND_MASK)
				printf(" FIN is on. ");

			decode = buf[1];
			if(decode && FIN_AND_MASK)
				printf("Masked is on. ");
			int payload_len = (PAYLOAD_LEN_MASK & decode);
			printf("Payload len: %d. ", payload_len);

			for(int i = 0; i < 4; ++i) {
				payload_mask[i] = buf[i+2];
			}
			
			for(int i = 0; i < payload_len; ++i) {
				message[i] = buf[i+6] ^ payload_mask[i % 4];
			}
			Write(1, message, payload_len);
			
			printf(" Ssize: %ld.", ssize);
			printf("\n");

			buf[0] = FIN_AND_MASK | 0x1;
			buf[1] = PAYLOAD_LEN_MASK & payload_len;
			for(int i = 0; i < payload_len; ++i)
				buf[2+i] = message[i];
			Write(sockfd, buf, payload_len+2);
		};
	};

	return 0;
}
