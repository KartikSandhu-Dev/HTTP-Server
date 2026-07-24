#include "http/request.h"
#include <string.h>

const char *http_request_header(HttpRequest *request, const char *name) {
	for(size_t i = 0; i < request->header_count; i++) {
		HttpHeader *header = &request->headers[request->header_count];
		if(strcmp(header->name, name) == 0) {
			return header->value;
		}
	}
	return NULL;
}