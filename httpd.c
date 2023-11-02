#include <arpa/inet.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#if   defined(__OpenBSD__)
#include <netinet/in.h>
#endif

#include "config.h"

void error(char * fmt) {
	fprintf(stdout, "%s\n", fmt);
	if(errno != 0) {
		fprintf(stdout, "(errno = %d) : %s\n", errno, strerror(errno));
	}
	fflush(stdout);
	exit(1);
}

int main() {
	uint8_t sockfd;
	uint8_t connfd;
	uint8_t recvline[BUFFER];
	uint8_t n;

	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;

	socklen_t clientaddr_len;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		error("Failed creating socket.");
	}

	//TODO: cast of void * 1 needs fix
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void *) 1, sizeof(int)) < 0 ) {
		error("unable to setsockopt");
	}

	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_family      = AF_INET;
	serveraddr.sin_port        = htons(PORT);

	if(bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) {
		error("Error while binding file descriptor.");
	}

	if(listen(sockfd, MAX_CONNECTIONS) < 0 ) {
		error("Listen error.");
	}

	printf("Listening on: localhost:%d\n", PORT);

	while(1) {
		connfd = accept(sockfd, (struct sockaddr *) &clientaddr, &clientaddr_len);

		while((n = read(connfd, recvline, BUFFER - 1)) > 0){
			printf("\n%s\n", recvline);
			if (recvline[n - 1] == '\n') {
				break;
			}

			memset(recvline, 0, BUFFER);
		}
		close(connfd);
	}

	return 0;
}
