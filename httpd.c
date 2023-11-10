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
#include "httpd.h"

void error(char * fmt) {
	fprintf(stdout, "%s\n", fmt);
	if(errno != 0) {
		fprintf(stdout, "(errno = %d) : %s\n", errno, strerror(errno));
	}
	fflush(stdout);
	exit(1);
}

void build_response(uint8_t connfd, const char *status, const char *content) {
	ssize_t bytes_written;
	char response[strlen(status) + strlen(CONTENT_TYPE_PLAIN) + strlen(content) + 1];
	sprintf(response, "%s%s%s", status, CONTENT_TYPE_PLAIN, content);

	bytes_written = write(connfd, response, strlen(response));
	if (bytes_written == -1) {
		error("Error writing to socket");
	}
}

void handle_request(uint8_t connfd, const char *request) {
	char method[10], path[255];
	sscanf(request, "%s %s", method, path);

	if (strcmp(path, "/") == 0) {
		build_response(connfd, HTTP_200, "Hello, World!");
	} else {
		build_response(connfd, HTTP_404, "404 Not Found");
	}
}

int main() {
	char recvline[BUFFER];

	struct sockaddr_in clientaddr;
	struct sockaddr_in serveraddr;

	uint8_t connfd;
	uint8_t n;
	uint8_t opt = 1;
	uint8_t sockfd;

	socklen_t clientaddr_len;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		error("Failed creating socket.");
	}

	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0 ) {
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
			printf("%s\n", recvline);
			if (recvline[n - 1] == '\n') {
				break;
			}
			memset(recvline, 0, BUFFER);
		}

		handle_request(connfd, recvline);
		close(connfd);
	}
	return 0;
}
