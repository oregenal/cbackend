#define PORT 56765
#define BUFFER_SIZE 2048

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(void)
{
	char str[BUFFER_SIZE];
	int sock, err;
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1) {
		perror("Socket error.");
		return 1;
	}

	err = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
	if(err == -1) {
		perror("Connect error.");
		return 2;
	}

	write(sock, "Hello!\n", 7);

	err = read(sock, str, BUFFER_SIZE);
	if(err == -1) {
		perror("Read error.");
		return 3;
	}

	printf("%s", str);
	
	close(sock);

	return 0;
}
