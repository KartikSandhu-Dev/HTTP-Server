#include "app/router.h"
#include "app/handler.h"
#include "common.h"
#include <linux/limits.h>

void router_handle(HttpRequest *request, HttpResponse *response) {
	char path[PATH_MAX];
	if(strcmp(request->path, "/") == 0)
		snprintf(path, sizeof(path), "public/index.html");
	else
		snprintf(path, sizeof(path), "public%s", request->path);

	if(handle_file(request, response, path))
		return;

	handle_not_found(request, response);
}
