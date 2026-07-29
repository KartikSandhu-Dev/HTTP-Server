#include "net/server.h"
#include "app/router.h"
#include "http/connection.h"
#include "http/response.h"
#include "net/socket.h"

#include "http/parser.h"
#include "common.h"

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <unistd.h>

int server_start(const ServerConfig *config) {
	int server = socket_create_server(config);

	while(1) {
		int client = accept_client(server);
		if(client < 0) continue;

		int yes = 1;
		setsockopt(client, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes));

		handle_client(client);
	}
}

void handle_client(int client) {
	HttpConnection conn = {0};
	memset(&conn, 0, sizeof(conn));

	conn.client = client;

	http_parser_init(&conn.parse, &conn.request);;

	while(1) {
		if(conn.buffer_len >= sizeof(conn.buffer))
			return;

		int bytes = recv(
			client, 
			conn.buffer + conn.buffer_len, 
			sizeof(conn.buffer) - conn.buffer_len - 1, 
			0
		);

		if(bytes <= 0) return;

		conn.buffer_len+=bytes;
		conn.buffer[conn.buffer_len] = '\0';

		HttpParseResult result = http_parser_parse(
			&conn.parse,
			&conn.request,
			conn.buffer,
			conn.buffer_len
		);

		switch (result) {
			case HTTP_RESULT_OK:
				http_response_init(&conn.response);
				
				router_handle(&conn.request, &conn.response);

				http_response_send(client, &conn.response);

				printf("%s %s %s\n", 
					conn.request.method,
					conn.request.path,
					conn.request.version
				);
				for(size_t i = 0; i < conn.request.header_count; i++) {
					printf("%s: %s\n", conn.request.headers[i].name, conn.request.headers[i].value);
				}
				printf("%.*s\n", (int)conn.request.content_length, conn.request.body);
				
				close(client);
				return;
			case HTTP_RESULT_NEED_MORE:
				continue;
			case HTTP_RESULT_ERROR:
				return;
			}
        }
}
