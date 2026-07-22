#include "net/server.h"
#include "net/socket.h"
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

		char *buffer = "HTTP/1.1 200 OK\r\n"
						"Content-Length: 13\r\n"
						"Content-Type: text/plain\r\n"
						"\r\n"
						"Hello, World!\n";

		send(client, buffer, strlen(buffer), 0);

		int bytes = recv(client, client_buffer, 1024, 0);
		client_buffer[bytes] = '\0';

		printf("%s", client_buffer);
	}
}