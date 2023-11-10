#define RESPONSE_PROTOCOL "HTTP/1.1"
#define CONTENT_TYPE_PLAIN "Content-Type: text/plain\r\n\r\n"

#define HTTP_200 RESPONSE_PROTOCOL " 200 OK\n\n"
#define HTTP_201 RESPONSE_PROTOCOL " 201 Created\n\n"
#define HTTP_404 RESPONSE_PROTOCOL " 404 Not Found\n\n"
#define HTTP_500 RESPONSE_PROTOCOL " 500 Internal Server Error\n\n"
