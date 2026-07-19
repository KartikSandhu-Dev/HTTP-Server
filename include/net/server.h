#ifndef SERVER_H
#define SERVER_H

#include "common.h"

typedef struct ServerConfig {
	char *port;
	int backlog; 
} ServerConfig;

int server_init(ServerConfig *config);

int server_start(ServerConfig *config);

#endif