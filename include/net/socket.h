#ifndef SOCKET_H
#define SOCKET_H

#include "net/server.h"

int socket_create_server(ServerConfig *config);
int accept_client();
void close_socket();

#endif