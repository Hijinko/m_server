#define _GNU_SOURCE
//#include <cserver.h>
#include "../include/cserver.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

/*
 * @brief creates a server
 * @param type the type of server
 * @param p_protocol the protocol to bind to the server
 * @return the server results
 */
struct addrinfo * cserver_create(int socktype, char * p_protocol)
{
    struct addrinfo hints = {0};
    hints.ai_socktype = socktype;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    struct addrinfo * p_results = {0};
    int err = getaddrinfo(NULL, p_protocol, &hints, &p_results);
    if (!err){
        // successful getting addrinfo
        return p_results;
    } else {
        // could not create results with getaddrinfo
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err)); 
        return NULL;
    }
}


/*
 * @brief creates a socket and binds the server to the socket
 * @param p_server the socket addrinfo to create the socket for
 * @return the socket that was created else -1 on error
 */
int cserver_socket_bind(struct addrinfo * p_server)
{
     // connect and bind to a socket
    int sfd = socket(p_server->ai_family,
                     p_server->ai_socktype,
                     p_server->ai_protocol);
    // enable reuseaddr flag on the socket
    int enable = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
    if (0 < sfd){
        if (0 == bind(sfd, p_server->ai_addr, p_server->ai_addrlen)){
            return sfd;
        } else {
            // could not bind on a socket
            perror("bind");
        }
    } else{
        // could not create the socket
        perror("sfd");
    }
    return -1;
}

/*
 * @brief gets the numeric address and service of the server
 * @param p_server the server to get the address for
 * @param p_host allocated space to put the address of the server
 * @param p_service allocated space to put the service of the server
 * @return an allocated string that is the servers address
 */
int cserver_info(struct addrinfo * p_server, char * p_host, char * p_service)
{
    char host[NI_MAXHOST] = {'\0'}, service[NI_MAXSERV] = {'\0'};
    int err = getnameinfo(p_server->ai_addr, p_server->ai_addrlen,
                          host, NI_MAXHOST, service, NI_MAXSERV,
                          NI_NUMERICHOST | NI_NUMERICSERV);
    if (!err){
        strncat(p_host, host, strlen(host));
        strncat(p_service, service, strlen(service));
        return 0;
    } else {
        fprintf(stderr, "getnameinfo: %s\n", gai_strerror(err));
        return err;
    }
}

/*
 * @brief listens for a connection
 * @param sfd the socket to list on
 * @return the socket that a connection was accepted on else -1 on error
 */
int cserver_wait_connection(int sfd)
{
    struct sockaddr_storage client = {0};
    socklen_t client_size = sizeof(client);
    int cfd = accept(sfd, (struct sockaddr *)&client, &client_size);     
    if (0 < cfd){
       return cfd; 
    } else {
        // could not accept a connection
        perror("accept");
        return -1;
    }
}