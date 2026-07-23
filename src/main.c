#include "net/server.h"
#include "config.h"

#include <stddef.h>
#include <sys/socket.h>

int main() {
	const ServerConfig sc = {
		.ip = IP,  // use any the os provides of the host
		.port = PORT,
		.backlog = SOMAXCONN, // a constant decided by the os
	};

	server_start(&sc);

	return 0;
}