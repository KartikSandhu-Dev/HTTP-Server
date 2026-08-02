#ifndef ROUTER_H
#define ROUTER_H

#include "http/request.h"
#include "http/response.h"

typedef void (*RouteHandler)(HttpRequest *request, HttpResponse *response);

typedef struct Route {
	const char *method;
	const char *path;
	RouteHandler route_handler;
} Route;

void router_dispatch(HttpRequest *request, HttpResponse *response);

#endif
