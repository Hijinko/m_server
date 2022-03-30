#ifndef _HANDLE_H
#define _HANDLE_H
#include <queue.h>
typedef struct
{
    queue * p_queue;
} handle_storage_t;
void handle_add_signal(int signal, void (* hanlder)(int sig));
void * handle_connection(void * p_client);
void * handle_thread(void * arg);
#endif