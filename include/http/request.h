#ifndef REQUEST_H
#define REQUEST_H

#include "config.h"
#include <stddef.h>

typedef struct HttpHeader {
	char name[HEADER_NAME_LEN];
	char value[HEADER_VALUE_LEN];
} HttpHeader;

typedef struct HttpRequest {
	char method[METHOD_LEN];
	char path[PATH_LEN];
	char version[VERSION_LEN];

	HttpHeader headers[MAX_HEADERS];
	size_t header_count;

	char *body;	
	size_t content_length;
} HttpRequest;

#endif