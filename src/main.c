#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include "erproc.h"
#include "str_search_ptrn.h"
#include "accept_key.h"

#define HIGHSCORE_FILE "highscore"
#define HIGHSCORE_BUFFER_SIZE 1024

const char *response = "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: ";
const char *socket_request = "Sec-WebSocket-Key:";

enum {
	SERVER_PORT = 56765,
	BUFFER_SIZE = 2048,
	REQUEST_KEY_SIZE = 24,
	FIN_AND_MASK = 0x80, /* 0b10000000 */
	TEXT_OPCODE = 0x1,
	CLOSE_OPCODE = 0x8,
	PAYLOAD_LEN_MASK = 0x7f /* 0b01111111 */
};

int main(void)
{
	int opt = 1;
	char buf[BUFFER_SIZE];
	char request_key[REQUEST_KEY_SIZE];
	char response_key[MESSAGE_SIZE];
	struct sockaddr_in addr;
	int response_len = strlen(response);
	char payload_mask[4];
	char highscore[HIGHSCORE_BUFFER_SIZE];
	int fds[FD_SETSIZE];

	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERVER_PORT);
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	int highscore_file = open(HIGHSCORE_FILE, O_RDWR|O_CREAT, 0666);
	ssize_t highscore_len = read(highscore_file, highscore, HIGHSCORE_BUFFER_SIZE);
#ifndef NDEBUG
	printf("Readed from file: %ld.\n", highscore_len);
	printf("HighScore: %s.\n", highscore);
#endif
	int ls = Socket(AF_INET, SOCK_STREAM, 0);
	
	Setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	Bind(ls, (struct sockaddr *) &addr, sizeof(addr));

	Listen(ls, 16);
#ifndef NDEBUG
	printf("Server started on port %d.\n", SERVER_PORT);
#endif
	int count = 0;

	for(;;) {
		fd_set readfds;
		int max_d = ls;

		FD_ZERO(&readfds);
		FD_SET(ls, &readfds);

		for(int i = 0; i < count; ++i) {
			FD_SET(fds[i], &readfds);
			if(max_d < fds[i])
				max_d = fds[i];
		}

		int res = select(max_d + 1, &readfds, NULL, NULL, NULL);
		
		if(res == -1) {
			if(errno == EINTR) {
				/* handle resived signal */
			} else {
				/* error handle */
			}
			continue; /* no point to check fds */
		}
		if(res == 0) {
			/* handle case time-out */
			continue; /* no point to check fds */
		}
		if(FD_ISSET(ls, &readfds)) {
			int sockfd = Accept(ls, NULL, NULL);

			if(sockfd < FD_SETSIZE) {
				ssize_t ssize = Read(sockfd, buf, BUFFER_SIZE);

				int pos = str_search_ptrn(socket_request, buf, ssize);

				for(int i = 0; i < REQUEST_KEY_SIZE; ++i)
					request_key[i] = buf[pos+19+i];

				accept_key_generator(request_key, response_key);
				
				Write(sockfd, response, response_len);
				Write(sockfd, response_key, MESSAGE_SIZE);
				Write(sockfd, "\r\n\r\n", 4);

				buf[0] = FIN_AND_MASK | TEXT_OPCODE;
				buf[1] = PAYLOAD_LEN_MASK & highscore_len;
				for(int i = 0; i < highscore_len; ++i)
					buf[2+i] = highscore[i];
				Write(sockfd, buf, highscore_len + 2);

				fds[count] = sockfd;
				++count;
			} else {
				Close(sockfd);
			}

		}

		for(int i = 0; i < count; ++i) {
			if(FD_ISSET(fds[i], &readfds)) {
#ifndef NDEBUG
				ssize_t ssize = Read(fds[i], buf, BUFFER_SIZE);
#endif

#ifdef NDEBUG
				Read(fds[i], buf, BUFFER_SIZE);
#endif

				uint8_t decode = buf[0];
				if((decode & CLOSE_OPCODE) == CLOSE_OPCODE) {
#ifndef NDEBUG
					printf("Close connection.\n");
#endif
					Close(fds[i]);

					for(int r = i; r < count - 1; ++r) {
						fds[r] = fds[r + 1];
					}
					--count;
					--i;
					continue;
				}

				if((decode & TEXT_OPCODE) != TEXT_OPCODE) {
#ifndef NDEBUG
					printf("Non text payload.\n");
#endif
					continue;
				}
#ifndef NDEBUG
				if(decode && FIN_AND_MASK)
					printf(" FIN is on. ");
#endif

				decode = buf[1];
#ifndef NDEBUG
				if(decode && FIN_AND_MASK)
					printf("Masked is on. ");
#endif
				highscore_len = (PAYLOAD_LEN_MASK & decode);
#ifndef NDEBUG
				printf("Payload len: %zu. ", highscore_len);
#endif

				for(int i = 0; i < 4; ++i)
					payload_mask[i] = buf[i+2];
				
				for(int i = 0; i < highscore_len; ++i)
					highscore[i] = buf[i+6] ^ payload_mask[i % 4];
#ifndef NDEBUG
				Write(1, highscore, highscore_len);
				
				printf(" Ssize: %ld.", ssize);
				printf("\n");
#endif
				lseek(highscore_file, 0, SEEK_SET);
				Write(highscore_file, highscore, highscore_len);

				buf[0] = FIN_AND_MASK | TEXT_OPCODE;
				buf[1] = PAYLOAD_LEN_MASK & highscore_len;
				for(int i = 0; i < highscore_len; ++i)
					buf[2+i] = highscore[i];
				for(int i = 0; i < count; ++i) {
					Write(fds[i], buf, highscore_len+2);
				}
			}
		}
	}

	return 0;
}
