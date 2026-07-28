#include "app/handler.h"
#include "common.h"
#include "http/response.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

void handle_response_file(HttpResponse *response, const char *path) {
	int fd = open(path, O_RDONLY);
	if(fd < 0) {
		http_response_status(response, 404);
		http_response_body(response, "Not Found", 9);
	}
	
	struct stat s;
	fstat(fd, &s);
	
	char *body = mmap(NULL, s.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if(!body) {
		http_response_status(response, 500);
		http_response_body(response, "Internal Server Error", 21);
	}
	http_response_body(response, body, s.st_size);

	close(fd);
}

void handle_home(HttpRequest *request, HttpResponse *response) {
	(void)request;
	http_response_status(response, 200);
	
	http_response_header(
		response, 
		"Content-Type",
		"text/html"
	);
	
	handle_response_file(response, "public/index.html");
}

void handle_not_found(HttpRequest *request, HttpResponse *response) {
	(void)request;
	http_response_status(response, 404);

	http_response_header(
		response,
		"Content-Type",
		"text/html"
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
