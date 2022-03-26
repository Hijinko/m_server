#include <list.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
 * @brief initializes a list node and assigns the data to it
 * @param p_data the data to assign to the node
 * @return NULL on error else an allocated list node
 */
static list_node * list_node_init(const void * p_data)
{
    list_node * p_node = calloc(1, sizeof(*p_node));
    if (NULL != p_node){
        // only assign the node values if it was succesffuly allocated
        p_node->p_data = p_data;
        p_node->p_prev = NULL;
        p_node->p_next = NULL;
    } else {
        // the node was not succesfully allocated
        perror("CALLOC");
    }
    return p_node;
}

/*
 * @brief a compare function for when the node data is supposed to be strings
 * @param p_string1 the first piece of data to analyze
 * @param p_string2 the second piece of data to analyze
 * @return -1, 0, 1 if p_string1 is less than, equal to 
 *  or greater than p_string2 or -2 on error
 */
int list_str_compare(const void * p_string1, const void * p_string2)
{
    int err = -2;
    if ((NULL != p_string1) && (NULL != p_string2)){
        // can only compare the strings if the data is not NULL
        err = strcmp((char *)p_string1, (char *)p_string2);
    }
    return err;
}

/*
 * @brief initializes a list structure
 * @param destroy a function used to destroy the data in a list node
 * @param compare a function used to compare the data in each node
 * @return NULL on error else an allocated list structure
 */
list * list_init(void * (* destroy)(const void * p_data), 
                 int (* compare)(const void * p_data1, const void * p_data2))
{
    list * p_list = calloc(1, sizeof(*p_list));
    if(NULL != p_list){
        // only assign th values if the list was successfuly allocated
        p_list->num_nodes = 0;
        p_list->p_head = NULL;
        p_list->p_tail = NULL;
        p_list->destroy = destroy;
        p_list->compare = compare;
    } else {
        // was not able to allocate the list
        perror("CALLOC");
    }
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
    // create the node
    list_node * p_node = list_node_init(p_data);
    // if the node is the first node then its both the head and tail
    if (0 == p_list->num_nodes){
        p_list->p_head = p_node;
        p_list->p_tail = p_node;
    } else {
        // the node is now the new tail and the old tail is moved 
        list_node * p_old_tail = p_list->p_tail;
        p_old_tail->p_next = p_node;
        p_node->p_prev = p_old_tail;
        p_list->p_tail = p_node;
    }
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
list_node * list_search(list * p_list, const void * p_data)
{
    for (list_node * p_node = p_list->p_head; p_node; p_node = p_node->p_next){
        if (0 == p_list->compare(p_node->p_data, p_data)){
            return p_node;
        }
    }
    return NULL;
}

/*
 * @brief removes data from a list, if the destroy of the list is not NULL,
  this function will run the destroy function on the data
 * @param p_list the list to remove the data from
 * @param p_data the data to remove fro the list
 * @return -1 on error or if the data is not in the list else 0
 */ 
int8_t list_remove(list * p_list, const void * p_data)
{
    uint8_t ret = -1;
    list_node * p_node = list_search(p_list, p_data);
    if (NULL != p_node){
        list_node * p_prev = p_node->p_prev;
        list_node * p_next = p_node->p_next;
        if (p_list->p_head == p_node){
            // check if node is at the head
            p_list->p_head = p_next;
        } else if (p_list->p_tail == p_node){
            p_list->p_tail = p_prev;
        }
        // restructure the list
        if (NULL != p_prev){
            p_prev->p_next = p_next;
        }
        if (NULL != p_next){
            p_next->p_prev = p_prev;
        }
        ret = 0;
        // now that the list is restructured remove the node
        if (NULL != p_list->destroy){
            p_list->destroy(p_node->p_data);
        }
        free(p_node);
    }
    p_list->num_nodes--;
    return ret;
}

/*
 * @brief destroys a list, frees the memory allocated by the list and all its
  nodes. if the destroy of the list is not NULL, this function will run the 
  destroy function on all the data in the list
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