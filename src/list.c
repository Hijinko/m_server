#include <list.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

static list_node * list_node_init(const void * p_data)
{
    list_node * p_node = calloc(1, sizeof(*p_node));
    p_node->p_data = p_data;
    p_node->p_prev = NULL;
    p_node->p_next = NULL;
    return p_node;
}

/*
 * @brief initializes a list structure
 * @param p_destroy a function used to destroy the data in a list node
 * @param p_compare a function used to compare the data in each node
 * @return NULL on error else an allocated list structure
 */
list * list_init(void (* p_destroy)(const void * p_data), 
                 void (* p_compare)(const void * p_data1, 
                                    const void * p_data2))
{
    list * p_list = calloc(1, sizeof(*p_list));
    p_list->num_nodes = 0;
    p_list->p_head = NULL;
    p_list->p_tail = NULL;
    p_list->p_destroy = p_destroy;
    p_list->p_compare = p_compare;
    return p_list;
}

/*
 * @brief adds data to the end of a list structure
 * @param p_list the list to append the data to
 * @param p_data the data to append to the list
 * @return NULL on error else a pointer to the list node that holds the data
 */
list_node * list_append(list * p_list, const void * p_data)
{
    // create the new node
    list_node * p_node = list_node_init(p_data);
    // check if this node is the new head
    if (0 == p_list->num_nodes){
        p_list->p_head = p_node;
    } else {
        // check if inserting the tail node
        if (NULL == p_list->p_tail){
            p_list->p_tail = p_node;
            p_node->p_prev = p_list->p_head;
            p_list->p_head->p_next = p_node;
        } else{
            // inserting after the tail so update the new tail
            p_list->p_tail->p_next = p_node;
            p_node->p_prev = p_list->p_tail;
            p_list->p_tail = p_node;
        }
    }
    // increase the number of nodes and return the node
    p_list->num_nodes++;
    return p_node;
}

/*
 * @brief searches a list for data and retrieves the node that holds the data
 * @param p_list the list to search through
 * @param p_data the data to search for in the list
 * @return NULL on error or if the data does not exist in the list else a
 *  pointer to the list node that holds the data
 */
list_node * list_search(list * p_list, const void * p_data);

/*
 * @brief removes data from a list, if the p_destroy of the list is not NULL,
  this function will run the p_destroy function on the data
 * @param p_list the list to remove the data from
 * @param p_data the data to remove fro the list
 */ 
int8_t list_remove(list * p_list, const void * p_data);

/*
 * @brief destroys a list, frees the memory allocated by the list and all its
  nodes. if the p_destroy of the list is not NULL, this function will run the 
  p_destroy function on all the data in the list
 * @param p_list the list to destroy
 */
void list_destroy(list * p_list)
{
    for (size_t index = 0; index < p_list->num_nodes; index++){
        // get the old head
        list_node * p_old_head = p_list->p_head;
        if (NULL != p_old_head){
            // move the new head;
            list_node * p_new_head = p_old_head->p_next;
            p_list->p_head = p_new_head;
            // free the old head;
            free(p_old_head);
        }
    }
    free(p_list);
}