#include "net/socket.h"
#include "net/server.h"
#include "common.h"

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <unistd.h>

int socket_create_server(const ServerConfig *config) {
	int server = -1; // the server socket

	int status;
	struct addrinfo hints;
	struct addrinfo *servinfo, *next;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	status = getaddrinfo(config->ip, config->port, &hints, &servinfo);
	if(status != 0) {
		fprintf(stderr, "%s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	}

	next = servinfo;

	while(next) {
		int sock = socket(next->ai_family, next->ai_socktype, next->ai_protocol);
		if(sock < 0) {
			perror("socket");
			continue;
		}

		int yes = 1;
		setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

		int bind_status = bind(sock, next->ai_addr, next->ai_addrlen);
		if(bind_status < 0) {
			perror("bind");
		} else {
			server = sock;
			break;
		}

		next = next->ai_next;
		close(sock);
	}

	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	if(getnameinfo(next->ai_addr, next->ai_addrlen, 
					host, sizeof(host),
					service, sizeof(service),
					NI_NUMERICHOST | NI_NUMERICSERV) == 0) {

		printf("Listening on %s:%s\n", host, service);
	}


	if(server < 0) {
		fprintf(stderr, "Couldnt initialise server");
		exit(EXIT_FAILURE);
	}

	int listen_status = listen(server, config->backlog);
	if(listen_status < 0) {
		perror("listen");
		close(server);
		freeaddrinfo(servinfo);
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(servinfo);

	return server;
}

int accept_client(int server_fd) {
	int sock = accept(server_fd, NULL, NULL);

	if(sock < 0) {
		perror("accept");
		return -1;
	}

	return sock;
}
