#ifndef HANDLER_H
#define HANDLER_H

#include "http/request.h"
#include "http/response.h"
#include <stdbool.h>

void handle_response_file(HttpResponse *response, const char *path);

void handle_home(HttpRequest *request, HttpResponse *response);

void handle_not_found(HttpRequest *request, HttpResponse *response);

#endif
