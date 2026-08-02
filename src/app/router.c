#include "app/router.h"
#include "app/handler.h"
#include "common.h"
#include <linux/limits.h>
#include <stddef.h>
#include <string.h>

#define ROUTE_COUNT (sizeof(routes)/sizeof(routes[0]))

static Route routes[] = {
	{
		"POST",
		"/login",
		handle_login	
	},
	{
		"GET",
		"/search",
		handle_search
	},
	{
		"GET",
		"/time",
		handle_time
	},
};

void router_dispatch(HttpRequest *request, HttpResponse *response) {
	// DYNAMIC ROUTES
	for(size_t i = 0; i < ROUTE_COUNT; i++) {
		if(strcmp(request->method, routes[i].method) != 0)
			continue;
		
		if(strcmp(request->path, routes[i].path) != 0)
			continue;

		routes[i].route_handler(request, response);
		return;
	}

	// ROUTE PARAMETERS
	if(strncmp(request->path, "/users/", 7) == 0){
		handle_user(request, response);
		return;
	}
	
	// STATIC ROUTE
	char path[PATH_MAX];
	if(strcmp(request->path, "/") == 0)
		snprintf(path, sizeof(path), "public/index.html");
	else
		snprintf(path, sizeof(path), "public%s", request->path);

	if(handle_file(request, response, path))
		return;

	handle_not_found(request, response);
}
