#ifndef HANDLER_H
#define HANDLER_H

#include "http/request.h"
#include "http/response.h"
#include <stdbool.h>

const char *http_content_type(const char *path);
bool handle_response_file(HttpResponse *response, const char *path);

bool handle_file(HttpRequest *request, HttpResponse *response, const char *path);

void handle_login(HttpRequest *request, HttpResponse *response);
void handle_search(HttpRequest *request, HttpResponse *response);
void handle_time(HttpRequest *request, HttpResponse *response);
void handle_user(HttpRequest *request, HttpResponse *response);

void handle_not_found(HttpRequest *request, HttpResponse *response);

#endif
