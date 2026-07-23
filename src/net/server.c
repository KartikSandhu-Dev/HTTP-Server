#include "net/server.h"
#include "net/socket.h"

#include "http/request.h"
#include "http/parser.h"
#include "common.h"

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>

int server_start(const ServerConfig *config) {
	int server = socket_create_server(config);

	while(1) {
		char client_buffer[1024];

		int client = accept_client(server);
		if(client < 0) continue;

		int yes = 1;
		setsockopt(client, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes));

		int bytes = recv(client, client_buffer, 1024, 0);
		client_buffer[bytes] = '\0';

		printf("%s", client_buffer);
	}
}