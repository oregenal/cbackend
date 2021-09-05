#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int Socket(int domain, int type, int protocol) {
	int res = socket(domain, type, protocol);
	if(res == -1) {
		perror("socket fail");
		exit(EXIT_FAILURE);
	}
	return res;
}

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
	int res = bind(sockfd, addr, addrlen);
	if(res == -1) {
		perror("bind fail");
		exit(EXIT_FAILURE);
	}
}

void Listen(int sockfd, int backlog) {
	int res = listen(sockfd, backlog);
	if(res == -1) {
		perror("listen fail");
		exit(EXIT_FAILURE);
	}
}

int Accept(int sockfd, struct sockaddr *restrict addr, 
		socklen_t *restrict addrlen) {
	int res = accept(sockfd, addr, addrlen);
	if(res == -1) {
		perror("accept fail");
		exit(EXIT_FAILURE);
	}
	return res;
}

void Setsockopt(int sockfd, int level, int optname, const void *optval,
		socklen_t optlen) {
	int res = setsockopt(sockfd, level, optname, optval, optlen);
	if(res == -1) {
		perror("socket options fail");
		exit(EXIT_FAILURE);
	}
}

int Read(int fd, void *buf, size_t count) {
	int res = read(fd, buf, count);
	if(res == -1) {
		perror("read fail");
		exit(EXIT_FAILURE);
	} else if(res == 0) {
		printf("EOF reached.\n");
	}
	return res;
}
