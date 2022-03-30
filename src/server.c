#define _GNU_SOURCE
#include <handle.h>
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
#include <sysexits.h>
#define THREAD_POOL_SIZE 10 
#define SRV_PORT "8888"

pthread_t thread_pool[THREAD_POOL_SIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER;
volatile bool b_running = true;

void sig_handler(int sig)
{
    (void)sig;
    b_running = false;
    // wake up threads for joining 
    pthread_cond_broadcast(&condition_var);
    for(uint8_t index = 0; index < THREAD_POOL_SIZE; index++)
    {
        pthread_join(thread_pool[index], NULL);
    }
}

void * thread_handler(void * arg)
{
    handle_storage_t * p_storage = arg;
    queue * p_queue = p_storage->p_queue;
    while(b_running){
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
    handle_add_signal(SIGINT, sig_handler);
    // initialize the storage handler
    handle_storage_t * p_storage = calloc(1, sizeof(*p_storage));
    // creat the queue for the threads
    queue * p_queue = queue_init(queue_compare_ints);
    p_storage->p_queue = p_queue;
    // create threads for the pool
    for(uint8_t index = 0; index < THREAD_POOL_SIZE; index++)
    {
        pthread_create(&thread_pool[index], NULL, thread_handler, p_storage);
    }

    // create the server to listen on
    struct addrinfo * p_server = cserver_create(SOCK_STREAM, SRV_PORT);
    int sfd = cserver_socket_bind(p_server);
    listen(sfd, MAX_LISTEN);
    char * p_host = calloc(NI_MAXHOST, sizeof(p_host));
    if (NULL == p_host)
    {
        perror("CALLOC");
        raise(SIGINT);
    }
    char * p_service = calloc(NI_MAXSERV, sizeof(p_service));
    if (NULL == p_host)
    {
        perror("CALLOC");
        raise(SIGINT);
    }
    cserver_info(p_server, p_host, p_service);
    freeaddrinfo(p_server);
    printf("[LISTENING] %s:%s\n", p_host, p_service);
    free(p_host);
    free(p_service);
    // run the server
    while(b_running)
    {
        // wait for a connection
        struct sockaddr_storage client;
        socklen_t client_sz = sizeof(client);    
        int cfd = accept4(sfd, (struct sockaddr*)&client, &client_sz, 0);
        int * p_client = &cfd;
        // enqueue the client file descriptor so that it can be handled by the
        // threads
        pthread_mutex_lock(&mutex);
        queue_enqueu(p_storage->p_queue, p_client);
        pthread_cond_signal(&condition_var);
        pthread_mutex_unlock(&mutex);
    }
    printf("[SHUTTING DOWN]\n");
    queue_destroy(p_queue);
    free(p_storage);
    close(sfd);
}