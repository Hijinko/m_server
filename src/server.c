#define _GNU_SOURCE
#include <queue.h>
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
#include <time.h>
#define THREAD_POOL_SIZE 10 

pthread_t thread_pool[THREAD_POOL_SIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER;

bool running = true;

void handler(int sig)
{
    (void)sig;
    running = false;
    // wake up threads for joining 
    pthread_cond_broadcast(&condition_var);
    for(uint8_t index = 0; index < THREAD_POOL_SIZE; index++)
    {
        pthread_join(thread_pool[index], NULL);
    }
}

void * handle_connection(void * p_client)
{
    int cfd = *(int *)p_client;
    char greeting[] = "hello\n";
    send(cfd, &greeting, strlen(greeting), 0);
    if(cfd > 0)
    {
        close(cfd);
    }
    return NULL;
}

void * thread_handler(void * arg)
{
    queue * p_queue = arg;
    while(running){
        int * p_client = NULL;
        if (NULL == (p_client = queue_dequeue(p_queue)))
        {
            pthread_cond_wait(&condition_var, &mutex);
            p_client  = queue_dequeue(p_queue);
        }
            pthread_mutex_unlock(&mutex);
        if (NULL != p_client)
        {
            handle_connection(p_client);
        }
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
    // creat the queue for the threads
    queue * p_queue = queue_init(queue_compare_ints);
    // create threads for the pool
    for(uint8_t index = 0; index < THREAD_POOL_SIZE; index++)
    {
        pthread_create(&thread_pool[index], NULL, thread_handler, p_queue);
    }

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
    while(running)
    {
        struct sockaddr_storage client;
        socklen_t client_sz = sizeof(client);    
        int cfd = accept4(sfd, (struct sockaddr*)&client, &client_sz, 0);
        int * p_client = &cfd;
        // pthread_mutex_lock(&mutex);
        pthread_mutex_lock(&mutex);
        queue_enqueu(p_queue, p_client);
        pthread_cond_signal(&condition_var);
        pthread_mutex_unlock(&mutex);
    }
    printf("[SHUTTING DOWN]\n");
    queue_destroy(p_queue);
    close(sfd);
    free(action);
    free(p_host);
    free(p_service);
}