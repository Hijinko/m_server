#define _GNU_SOURCE
#include <handle.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

void handle_add_signal(int signal, void (* handler)(int sig))
{
    struct sigaction * p_action = calloc(1, sizeof(*p_action));
    p_action->sa_handler = handler;
    sigaction(signal, p_action, NULL);
    free(p_action);
}
void * handle_connection(void * p_client)
{
    int * cfd = p_client;
    char greeting[] = "hello\n";
    send(*cfd, &greeting, strlen(greeting), 0);
    close(*cfd);
    return NULL;
}
void * handle_thread(void * arg);