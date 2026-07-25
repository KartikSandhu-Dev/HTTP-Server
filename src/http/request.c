#include "http/request.h"
#include <string.h>

/* EXAMPLE HTTP REQUEST

GET /index.html HTTP/1.1
Host: localhost:8080
User-Agent: Mozilla/5.0
Accept: text/html
Connection: keep-alive

*/

const char *http_request_header(HttpRequest *request, const char *name) {
	for(size_t i = 0; i < request->header_count; i++) {
		HttpHeader *header = &request->headers[request->header_count];
		if(strcmp(header->name, name) == 0) {
			return header->value;
		}
	}
	return NULL;
}