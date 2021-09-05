#ifndef ERPROC_H
#define ERPROC_H

#include <sys/socket.h>

int Socket(int domain, int type, int protocol);
void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
void Listen(int sockfd, int backlog);
int Accept(int sockfd, struct sockaddr *restrict addr,
		socklen_t *restrict addrlen);
void Setsockopt(int sockfd, int level, int optname, const void *optval,
		socklen_t optlen);
int Read(int fd, void *buf, size_t count);

#endif
