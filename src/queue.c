#include <queue.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

/*
 * @brief a prebuilt compare function for when the data in a queue is an int 
 * @param p_num1 the first number to compare
 * @param p_num2 the second number to compare
 * @return -1, 0, 1 if p_num1 is less than equal to or greater than p_num2
 */
int queue_compare_ints(const void * p_num1, const void * p_num2)
{
    const int * num1 = p_num1;
    const int * num2 = p_num2;
    // compares num1 to num2 and returns the appropriate value
    return (*num1 < *num2) ? -1 : ((*num1 == *num2) ? 0 : 1);
}

/*
 * @brief initialzes a queue data structure 
 * @param compare the function used to compare data in the queue
 * @return an initialized queue data structure
 */
queue * queue_init(int compare(const void * p_data1, const void * p_data2))
{
    queue * p_queue = calloc(1, sizeof(*p_queue));
    if (NULL != p_queue){
        // allocation was successful
        p_queue->p_stack = list_init(NULL, compare);
        if (NULL == p_queue->p_stack){
            // if the stack could not be allocated then free the queue 
            // and return NULL
            free(p_queue);
            return NULL;
        }
    } else {
        perror("CALLOC");
    }
    return p_queue;
}

/*
 * @brief destroys a queue data structure and frees any allocated memory
 * @param p_queue the queue to destroy
 */
void queue_destroy(queue * p_queue)
{
    if (NULL != p_queue){
        // can only free memory if the data is not NULL
        list_destroy(p_queue->p_stack);
        free(p_queue);
    }
}

/*
 * @brief enqueues data to a queues stack, adds the data to the top of the stack
 * @param p_queue the queue to enqueu to
 * @param p_data the data to enqueu to the stack
 * @return a pointer to the data on success else NULL on error
 */
void * queue_enqueu(queue * p_queue, void * p_data)
{
    if ((NULL != p_queue) && (NULL != p_data)){
        list_node * p_node = list_append(p_queue->p_stack, p_data);
        if (NULL != p_node){
            // appending to the queue was successful so return the data
            return p_node->p_data;
        }
    }
    return NULL;
}

/*
 * @brief dequeues data on the queues stack, removes data from the bottom of the
 *  queue
 * @param p_queue the queue to remove from the stack
 * @return the data that was dequed from the stack or NULL on error
 */
void * queue_dequeue(queue * p_queue)
{
    if ((NULL != p_queue) && (NULL != p_queue->p_stack->p_head)){
        void * p_data = p_queue->p_stack->p_head->p_data;
        if (0 == list_remove(p_queue->p_stack, p_data)){
            // removing the data from the queue was successful
            return p_data;
        }
    }
    return NULL;
}