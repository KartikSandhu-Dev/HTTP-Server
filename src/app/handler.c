#include "app/handler.h"
#include "common.h"
#include "config.h"
#include "http/response.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>

bool handle_response_file(HttpResponse *response, const char *path) {
	int fd = open(path, O_RDONLY);
	if(fd < 0) {
		http_response_status(response, 404);
		http_response_body(response, "Not Found", 9);
		return false;
	}
	
	struct stat s;
	if (fstat(fd, &s) < 0) {
		close(fd);

		http_response_status(response, 500);
		http_response_body(response, "Internal Server Error", 21);
		return false;
	}
	
	char *body = mmap(NULL, s.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if(body == MAP_FAILED) {
		http_response_status(response, 500);
		http_response_body(response, "Internal Server Error", 21);
		return false;
	}
	
	http_response_status(response, 200);
	http_response_body(response, body, s.st_size);
	close(fd);
	return true;
}

bool handle_file(HttpRequest *request, HttpResponse *response, const char *path) {
	(void)request;
	
	http_response_header(
		response,
		HTTP_HEADER_CONTENT_TYPE,
		http_content_type(path)
	);

	http_response_header(
		response,
		HTTP_HEADER_CONNECTION,
		HTTP_CONNECTION_CLOSE
	);
	
	http_response_header(
		response,
		HTTP_HEADER_SERVER,
		HTTP_SERVER_NAME
	);

	if(handle_response_file(response, path))
		return true;

	return false;
}

void handle_login(HttpRequest *request, HttpResponse *response) {
	http_response_status(response, 200);

	http_response_header(
		response,
		HTTP_HEADER_CONTENT_TYPE,
		HTTP_CONTENT_HTML
	);

	http_response_header(
		response,
		HTTP_HEADER_CONNECTION,
		HTTP_CONNECTION_CLOSE
	);
	
	http_response_header(
		response,
		HTTP_HEADER_SERVER,
		HTTP_SERVER_NAME
	);

	char html[] =
		"<h1>Login received</h1>";

	http_response_body(
		response,
		html,
		strlen(html)
	);

}

void handle_search(HttpRequest *request, HttpResponse *response) {
	http_response_status(response, 200);

	http_response_header(
		response,
		HTTP_HEADER_CONTENT_TYPE,
		HTTP_CONTENT_HTML
	);

	http_response_header(
		response,
		HTTP_HEADER_CONNECTION,
		HTTP_CONNECTION_CLOSE
	);
	
	http_response_header(
		response,
		HTTP_HEADER_SERVER,
		HTTP_SERVER_NAME
	);

	char html[] = 
		"<h1>Search Page</h1>";

	http_response_body(
		response,
		html,
		strlen(html)
	);
}

void handle_time(HttpRequest *request, HttpResponse *response) {
	http_response_status(response, 200);

	char json[256];
	time_t now = time(NULL);

	snprintf(json, sizeof(json),
		"{\"time\":\"%ld\"}",
		now
	);
	
	http_response_header(
		response,
		HTTP_HEADER_CONTENT_TYPE,
		HTTP_CONTENT_HTML
	);

	http_response_header(
		response,
		HTTP_HEADER_CONNECTION,
		HTTP_CONNECTION_CLOSE
	);
	
	http_response_header(
		response,
		HTTP_HEADER_SERVER,
		HTTP_SERVER_NAME
	);

	http_response_body(
		response,
		json,
		strlen(json)
	);

}

void handle_user(HttpRequest *request, HttpResponse *response) {
	http_response_status(response, 200);
	
	http_response_header(
		response,
		HTTP_HEADER_CONTENT_TYPE,
		HTTP_CONTENT_HTML
	);

	http_response_header(
		response,
		HTTP_HEADER_CONNECTION,
		HTTP_CONNECTION_CLOSE
	);
	
	http_response_header(
		response,
		HTTP_HEADER_SERVER,
		HTTP_SERVER_NAME
	);

	char html[256];

	snprintf(html, sizeof(html),
		"<h1>User page %s</h1>", request->path + 7
	);
	
	http_response_body(
		response,
		html,
		strlen(html)
	);
}

void handle_not_found(HttpRequest *request, HttpResponse *response) {
	(void)request;
	http_response_status(response, 404);

	http_response_header(
		response,
		HTTP_HEADER_CONTENT_TYPE,
		HTTP_CONTENT_HTML
	);
	
	http_response_header(
		response,
		HTTP_HEADER_CONNECTION,
		HTTP_CONNECTION_CLOSE
	);

	http_response_header(
		response,
		HTTP_HEADER_SERVER,
		HTTP_SERVER_NAME
	);

	static const char html[] = {
		"<html>"
        "<body>"
        "<h1>404 Not Found</h1>"
        "</body>"
        "</html>"
	};

	http_response_body(response, html, strlen(html));
}

const char *http_content_type(const char *path) {
	const char *ext = strrchr(path, '.');

	if(!ext) return HTTP_CONTENT_TEXT;

	if(strcmp(ext, ".html") == 0)
		return HTTP_CONTENT_HTML;
	
	if (strcmp(ext, ".css") == 0)
		return HTTP_CONTENT_CSS;

	if (strcmp(ext, ".js") == 0)
		return HTTP_CONTENT_JS;

	if (strcmp(ext, ".json") == 0)
		return HTTP_CONTENT_JSON;

	if (strcmp(ext, ".png") == 0)
		return HTTP_CONTENT_PNG;

	if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0)
		return HTTP_CONTENT_JPEG;

	if (strcmp(ext, ".svg") == 0)
		return HTTP_CONTENT_SVG;

    if (strcmp(ext, ".ico") == 0)
		return HTTP_CONTENT_ICO;
	
	return "application/octet-stream";
}
