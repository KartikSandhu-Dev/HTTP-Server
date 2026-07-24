#include "http/response.h"

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