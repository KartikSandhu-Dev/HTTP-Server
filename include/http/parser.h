#ifndef PARSER_H
#define PARSER_H

#include "request.h"
#include "common.h"

typedef enum HttpState {
	HTTP_STATE_METHOD,
	HTTP_STATE_PATH,
	HTTP_STATE_VERSION,
	HTTP_STATE_HEADER_NAME,
	HTTP_STATE_HEADER_VALUE,
	HTTP_STATE_BODY,
	HTTP_STATE_ERROR,
	HTTP_STATE_DONE,
} HttpState;

typedef struct HttpParser {
	char *buffer;
	size_t buffer_len;

	HttpState state;

	size_t pos;
	size_t token_pos;
} HttpParser;

typedef enum HttpParseResult {
	HTTP_RESULT_OK,
	HTTP_RESULT_NEED_MORE,
	HTTP_RESULT_ERROR,
} HttpParseResult;

typedef HttpParseResult (*HttpStateHandler)(
	HttpParser *hp, 
	HttpRequest *request
);

void http_parser_init(HttpParser *hp, HttpRequest *request);

// IN ORDER WITH THE HTTPSTATE ENUM.
HttpParseResult parse_method(HttpParser *hp, HttpRequest *request);
HttpParseResult parse_path(HttpParser *hp, HttpRequest *request);
HttpParseResult parse_version(HttpParser *hp, HttpRequest *request);
HttpParseResult parse_header_name(HttpParser *hp, HttpRequest *request);
HttpParseResult parse_header_value(HttpParser *hp, HttpRequest *request);
HttpParseResult parse_body(HttpParser *hp, HttpRequest *request);
HttpParseResult parse_done(HttpParser *hp, HttpRequest *request);
HttpParseResult parse_error(HttpParser *hp, HttpRequest *request);

HttpParseResult http_parser_parse(HttpParser *hp, HttpRequest *request, char *buffer, size_t len);

void change_state(HttpParser *hp, HttpRequest *request, HttpState new_state);
bool match_crlf(HttpParser *hp);


#endif 