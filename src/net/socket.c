#include "net/socket.h"
#include "net/server.h"
#include "common.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int socket_create_server(ServerConfig *config) {
	int status;
	struct addrinfo hints;
	struct addrinfo *servinfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	status = getaddrinfo(NULL, config->port, &hints, &servinfo);
	if(status != 0) {
		perror("getaddrinfo");
		exit(1);
	}

	

	freeaddrinfo(servinfo);
}