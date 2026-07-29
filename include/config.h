#ifndef CONFIG_H
#define CONFIG_H

#define IP NULL
#define PORT "8080"

#define CONNECTION_BUFFER_LEN 8196

#define METHOD_LEN 16
#define PATH_LEN 256
#define VERSION_LEN 16

#define MAX_HEADERS 32
#define HEADER_NAME_LEN 64
#define HEADER_VALUE_LEN 1024

#define HTTP_HEADER_CONTENT_TYPE "Content-Type"
#define HTTP_HEADER_CONNECTION   "Connection"
#define HTTP_HEADER_SERVER       "Server"

#define HTTP_CONNECTION_CLOSE "close"
#define HTTP_SERVER_NAME "tpServer"

// MIME types (CONTENT TYPES)
#define HTTP_CONTENT_HTML "text/html; charset=UTF-8"
#define HTTP_CONTENT_CSS  "text/css; charset=UTF-8"
#define HTTP_CONTENT_JS   "application/javascript; charset=UTF-8"
#define HTTP_CONTENT_JSON "application/json; charset=UTF-8"
#define HTTP_CONTENT_PNG  "image/png"
#define HTTP_CONTENT_JPEG "image/jpeg"
#define HTTP_CONTENT_SVG  "image/svg+xml"
#define HTTP_CONTENT_ICO  "image/x-icon"
#define HTTP_CONTENT_TEXT "text/plain; charset=UTF-8"

#endif
