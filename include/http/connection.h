#ifndef CONNECTION_H
#define CONNECTION_H

#include "http/parser.h"
#include "http/request.h"
#include "config.h"

typedef struct HttpConnection {
	int client;
	
	char buffer[CONNECTION_BUFFER_LEN];
	size_t buffer_len;

	HttpParser parse;
	HttpRequest request;
} HttpConnection;

#endif