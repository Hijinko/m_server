#ifndef _LIST_H
#define _LIST_H
#include <stdint.h>
#include <stdlib.h>

/*
 * @brief a node for a list structure
 * @param p_prev pointer to the node before the current node
 * @param p_next pointer to the node after the current node 
 * @param p_data the data that the node has
 */
typedef struct list_node {
    struct list_node * p_prev;
    struct list_node * p_next;
    const void * p_data;
} list_node;

/*
 * @brief a doubly linked list structure
 * @param p_head the head node of the list
 * @param p_tail the last node in the list
 * @param num_nodes the number of nodes in the list
 * @param p_destroy the function used to destroy the data that the nodes hold
 * @param p_compare the function used to compare the data that the nodes hold
 */
typedef struct list {
    list_node * p_head;
    list_node * p_tail;
    size_t num_nodes;
    void * (* p_destroy)(const void * p_data);
    int (* p_compare)(const void * p_data1, const void * p_data2);
} list;
int list_str_compare(const void * p_string1, const void * p_string2);
list * list_init(void * (* p_destroy)(const void * p_data), 
                 int (* p_compare)(const void * p_data1, const void * p_data2));
list_node * list_append(list * p_list, const void * p_data);
list_node * list_search(list * p_list, const void * p_data);
int8_t list_remove(list * p_list, const void * p_data);
void list_destroy(list * p_list);
#endif