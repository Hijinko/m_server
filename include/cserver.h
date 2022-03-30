#ifndef _CSERVER_H
#define _CSERVER_H
#define MAXBUFF 1024
#define MAX_LISTEN 20
#include <netdb.h>
struct addrinfo * cserver_create(int socktype, const char * p_protocol);
int cserver_socket_bind(struct addrinfo * p_server);
int cserver_info(struct addrinfo * p_server, char * p_host, char * p_service);
int cserver_wait_connection(int sfd);
#endif
