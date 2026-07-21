#include "net/server.h"
#include "net/socket.h"
#include <sys/socket.h>

int server_start(const ServerConfig *config) {
	int server = socket_create_server(config);

	while(1) {
		int client = accept_client(server);

		if(client < 0) continue;

		send(client, "Hello world!\n", 14, 0);
	}
}