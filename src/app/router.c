#include "app/router.h"
#include "app/handler.h"
#include "common.h"

void router_handle(HttpRequest *request, HttpResponse *response) {
	if(strcmp(request->path, "/") == 0) {
		handle_home(request, response);
		return;
	}

	handle_not_found(request, response);
}
