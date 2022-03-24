#define _GNU_SOURCE
#define _POSIX_C_SOURCE
#include <cserver.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <netdb.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>

bool running = true;

void handler(int sig)
{
    (void)sig;
    running = false;
}

void * handle_connection(void * arg)
{
    int cfd = *(int *)arg;
    char greeting[] = "hello\n";
    send(cfd, &greeting, strlen(greeting), 0);
    if(cfd > 0){
        close(cfd);
    }
    return NULL;
}

int main(int argc, char ** argv)
{
    (void)argc;
    (void)argv;
    // handle Ctrl-C
    struct sigaction * action = calloc(1, sizeof(*action));
    action->sa_handler = handler;
    sigaction(SIGINT, action, NULL);
    char port[] = "8888";
    // create the server to listen on
    struct addrinfo * p_server = cserver_create(SOCK_STREAM, port);
    int sfd = cserver_socket_bind(p_server);
    listen(sfd, MAX_LISTEN);
    char * p_host = calloc(NI_MAXHOST, sizeof(p_host));
    char * p_service = calloc(NI_MAXSERV, sizeof(p_service));
    cserver_info(p_server, p_host, p_service);
    freeaddrinfo(p_server);
    printf("[LISTENING] %s:%s\n", p_host, p_service);
    // run the server
    while(running){
        struct sockaddr_storage client;
        socklen_t client_sz = sizeof(client);    
        int cfd = accept4(sfd, (struct sockaddr*)&client, &client_sz, 0);
        pthread_t connection_thread;
        pthread_create(&connection_thread, NULL, handle_connection, &cfd);
        pthread_join(connection_thread, NULL);
    }
    printf("[SHUTTING DOWN]\n");
    close(sfd);
    free(action);
    free(p_host);
    free(p_service);
}