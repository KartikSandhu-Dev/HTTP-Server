#ifndef ROUTER_H
#define ROUTER_H

#include "http/request.h"
#include "http/response.h"

void router_handle(HttpRequest *request, HttpResponse *response);

#endif
