#include "net/server.h"
#include <sys/socket.h>
#include "common.h"

int main() {
	const ServerConfig sc = {
		.ip = NULL,  // use any the os provides of the host
		.port = "8080",
		.backlog = SOMAXCONN, // a constant decided by the os
	};

	server_start(&sc);

	return 0;
}