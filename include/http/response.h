#ifndef RESPONSE_H
#define RESPONSE_H

#include "config.h"
#include <stddef.h>

typedef struct HttpHeader_res {
	const char *name;
	const char *value;
} HttpHeader_res;

typedef struct HttpResponse {
	int status;

	HttpHeader_res headers[MAX_HEADERS];
	size_t header_count;

	const char *body;
	size_t body_length;
} HttpResponse;


void http_response_init(HttpResponse *response);
void http_response_status(HttpResponse *response, int status);
void http_response_header(HttpResponse *response, const char *name, const char *value);
void http_response_body(HttpResponse *response, const char *body, size_t len);

void http_response_send(int client, HttpResponse *response);

#endif