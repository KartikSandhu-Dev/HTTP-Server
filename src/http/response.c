#include "http/response.h"
#include "common.h"
#include "config.h"
#include <stdio.h>
#include <sys/mman.h>
#include <sys/socket.h>

/* EXAMPLE HTTP RESPONSE

HTTP/1.1 200 OK
Content-Type: text/html
Content-Length: 45

<html>
<body>
<h1>Hello World</h1>
</body>
</html>

*/

void http_response_init(HttpResponse *response) {
	response->status = 200;
	response->header_count = 0;
	response->body_length = 0;
	response->body = NULL;
}

void http_response_status(HttpResponse *response, int status) {
	response->status = status;
}

void http_response_header(HttpResponse *response, const char *name, const char *value) {
	if (response->header_count >= MAX_HEADERS)
		return;

	response->headers[response->header_count].name = name;
	response->headers[response->header_count].value = value;

	response->header_count++;
}

void http_response_body(HttpResponse *response, const char *body, size_t len) {
	response->body = body;
	response->body_length = len;
}

static const char *status_text(int status) {
	switch (status) {
		case 200: return "OK";
		case 201: return "Created";
		case 204: return "No Content";
		case 400: return "Bad Request";
		case 404: return "Not Found";
		case 500: return "Internal Server Error";
		default: return "Unkown";
	}
}

static void send_status(int client, HttpResponse *response) {
	char buffer[128];

	snprintf(buffer, sizeof(buffer), 
			"HTTP/1.1 %d %s\r\n", 
			response->status, status_text(response->status)
		);

	send(client, buffer, strlen(buffer), 0);
}

static void send_headers(int client, HttpResponse *response) {
	char buffer[HEADER_NAME_LEN + HEADER_VALUE_LEN];

	for(size_t i = 0; i < response->header_count; i++) {
		HttpHeader_res hd = response->headers[i];

		snprintf(buffer, sizeof(buffer), 
				"%s: %s\r\n",
				hd.name, hd.value
			);

		send(client, buffer, strlen(buffer), 0);
	}
}

static void send_content_length(int client, HttpResponse *response) {
	char buffer[128];

	snprintf(buffer, sizeof(buffer), 
		"%s: %zu\r\n", 
		CONTENT_LENGTH, response->body_length
	);

	send(client, buffer, strlen(buffer), 0);
}

static void send_blank_line(int client) {
	send(client, "\r\n", 2, 0);
}

static void send_body(int client, HttpResponse *response) {
	send(client, response->body, response->body_length, 0);
}

void http_response_send(int client, HttpResponse *response) {
	send_status(client, response);
	send_headers(client, response);
	send_content_length(client, response);
	send_blank_line(client);
	send_body(client, response);
	
	// freeing the body mmap
	munmap((void *)response->body, response->body_length);
}
