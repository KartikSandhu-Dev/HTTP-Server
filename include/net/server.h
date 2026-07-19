#ifndef SERVER_H
#define SERVER_H

#include "common.h"

typedef struct ServerConfig {
	const char *ip;
	uint16_t port;
	int backlog; 
} ServerConfig;

int server_start(ServerConfig *config);

#endif