#ifndef _QUEUE_H
#define _QUEUE_H
#include <list.h>
#include <stdlib.h>
typedef struct list stack;
typedef struct queue {
    stack * p_stack;
} queue;
int queue_compare_ints(const void * p_num1, const void * p_num2);
queue * queue_init(int compare(const void * p_data1, const void * p_data2));
void queue_destroy(queue * p_queue);
const void * queue_enqueu(queue * p_queue, const void * p_data);
const void * queue_dequeue(queue * p_queue);
#endif