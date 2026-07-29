#ifndef ROUTER_H
#define ROUTER_H

#include "http/request.h"
#include "http/response.h"

typedef void (*RouteHandler)(HttpRequest *request, HttpResponse *response, const char *data);

typedef struct Route {
	const char *method;
	const char *path;
	RouteHandler route_handler;
	const char *data;
} Route;

void router_handle(HttpRequest *request, HttpResponse *response);

#endif
