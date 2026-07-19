#include "net/server.h"
#include <sys/socket.h>

int main() {
	ServerConfig sc = {
		.port = "8080",
		.backlog = SOMAXCONN,
	};


}