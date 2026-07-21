#ifndef SOCKET_H
#define SOCKET_H

#include "net/server.h"

int socket_create_server(const ServerConfig *config);
int accept_client(int server_fd);

#endif