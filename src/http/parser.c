#include "http/parser.h"
#include "http/request.h"
#include "common.h"
#include "config.h"

static HttpStateHandler handlers[] = {
    parse_method,
    parse_path,
    parse_version,
    parse_header_name,
    parse_header_value,
    parse_body,
    parse_done,
    parse_error,
};

char current(HttpParser *hp) {
	return hp->buffer[hp->pos];
}

void advance(HttpParser *hp) {
	hp->pos++;
}

void http_parser_init(HttpParser *hp, HttpRequest *request) {
	memset(request, 0, sizeof(*request));
	memset(hp, 0, sizeof(*hp));

	hp->buffer_len = 0;
	hp->pos = 0;
	hp->token_pos = 0;
	hp->state = HTTP_STATE_METHOD;

	request->header_count = 0;
	request->content_length = 0;
}

HttpParseResult http_parser_parse(HttpParser *hp, HttpRequest *request, char *buffer, size_t len) {
	hp->buffer = buffer;
	hp->buffer_len = len;

	while(hp->pos < hp->buffer_len) {
		HttpParseResult result = handlers[hp->state](hp, request);

		if(result != HTTP_RESULT_NEED_MORE) {
			return result;
		}
	}

	return HTTP_RESULT_NEED_MORE;
}

static HttpParseResult push_char(HttpParser *hp, char *dest) {
	dest[hp->token_pos++] = current(hp);
	advance(hp);
	return HTTP_RESULT_NEED_MORE;
}

static HttpParseResult report_error(HttpParser *hp) {
	hp->state = HTTP_STATE_ERROR;
	return HTTP_RESULT_ERROR;
}

HttpParseResult parse_method(HttpParser *hp, HttpRequest *request) {
	if(hp->token_pos >= sizeof(request->method)-1)
		return report_error(hp);

	if(current(hp) == ' ') {
		change_state(hp, request, HTTP_STATE_PATH);
		advance(hp);
		return HTTP_RESULT_NEED_MORE;
	}

	if(!isupper(current(hp)))
		return report_error(hp);

	return push_char(hp, request->method);
}

HttpParseResult parse_path(HttpParser *hp, HttpRequest *request) {
	if(hp->token_pos >= sizeof(request->path)-1)
		return report_error(hp);

	if(current(hp) == ' ') {
		if(request->path[0] != '/')
			return report_error(hp);

		change_state(hp, request, HTTP_STATE_VERSION);
		advance(hp);
		return HTTP_RESULT_NEED_MORE;
	}

	return push_char(hp, request->path);
}

HttpParseResult parse_version(HttpParser *hp, HttpRequest *request) {
	if(hp->token_pos >= sizeof(request->version)-1)
		return report_error(hp);

	if(match_crlf(hp)) {
		change_state(hp, request, HTTP_STATE_HEADER_NAME);

		if(strncmp(request->version, "HTTP/", 5) != 0)
			return report_error(hp);

		return HTTP_RESULT_NEED_MORE;
	}

	return push_char(hp, request->version);
}

HttpParseResult parse_header_name(HttpParser *hp, HttpRequest *request) {
	if(hp->token_pos >= sizeof(request->headers[request->header_count].name)-1)
		return report_error(hp);

	if(match_crlf(hp)) {
		if(request->content_length > 0) {
			change_state(hp, request, HTTP_STATE_BODY);
			return HTTP_RESULT_NEED_MORE;
		}
		change_state(hp, request, HTTP_STATE_DONE);
		return HTTP_RESULT_OK;
	}

	if(isspace(current(hp)))
		return report_error(hp);

	if(current(hp) == ':') {
		change_state(hp, request, HTTP_STATE_HEADER_VALUE);
		advance(hp);
		return HTTP_RESULT_NEED_MORE;
	}

	return push_char(hp, request->headers[request->header_count].name);
}

HttpParseResult parse_header_value(HttpParser *hp, HttpRequest *request) {
	if(hp->token_pos >= sizeof(request->headers[request->header_count].value)-1)
		return report_error(hp);

	if(current(hp) == ' ' && hp->token_pos == 0) {
		advance(hp);
		return HTTP_RESULT_NEED_MORE;
	}

	if(match_crlf(hp)) {
		change_state(hp, request, HTTP_STATE_HEADER_NAME);

		if(request->header_count >= MAX_HEADERS)
			return report_error(hp);

		if(strcmp(request->headers[request->header_count].name, CONTENT_LENGTH) == 0) {
			request->content_length = strtoul(request->headers[request->header_count].value, NULL, 10);
		}

		request->header_count++;
		return HTTP_RESULT_NEED_MORE;
	}

	return push_char(hp, request->headers[request->header_count].value);
}

HttpParseResult parse_body(HttpParser *hp, HttpRequest *request) {
	size_t content_length = request->content_length;

	if(content_length > hp->buffer_len - hp->pos)
		return HTTP_RESULT_NEED_MORE;

	request->body = hp->buffer + hp->pos;

	hp->pos+=content_length;
	hp->state = HTTP_STATE_DONE;

	return HTTP_RESULT_OK;
}

HttpParseResult parse_done(HttpParser *hp, HttpRequest *request) {
	change_state(hp, request, HTTP_STATE_DONE);
	return HTTP_RESULT_OK;
}

HttpParseResult parse_error(HttpParser *hp, HttpRequest *request) {
	change_state(hp, request, HTTP_STATE_ERROR);
	return HTTP_RESULT_ERROR;
}

void change_state(HttpParser *hp, HttpRequest *request, HttpState new_state) {
	switch (hp->state) {
		case HTTP_STATE_METHOD:
			request->method[hp->token_pos] = '\0';
			break;
		case HTTP_STATE_PATH:
			request->path[hp->token_pos] = '\0';
			break;
		case HTTP_STATE_VERSION:
			request->version[hp->token_pos] = '\0';
			break;

		case HTTP_STATE_HEADER_NAME:
			request->headers[request->header_count].name[hp->token_pos] = '\0';
			break;

		case HTTP_STATE_HEADER_VALUE:
			request->headers[request->header_count].value[hp->token_pos] = '\0';
			break;

		default:
			break;
	}

	hp->state = new_state;
	hp->token_pos = 0;
}

bool match_crlf(HttpParser *hp) {
	if(hp->pos + 1 >= hp->buffer_len) 
		return false;

	if(hp->buffer[hp->pos] != '\r')
		return false;

	if(hp->buffer[hp->pos + 1] != '\n')
		return false;

	hp->pos+=2;

	return true;
}