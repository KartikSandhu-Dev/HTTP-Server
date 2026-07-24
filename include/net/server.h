#ifndef SERVER_H
#define SERVER_H

typedef struct ServerConfig {
	char *ip;
	char *port;
	int backlog; 
} ServerConfig;

int server_start(const ServerConfig *config);
void handle_client(int client);

#endif