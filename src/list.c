#include <list.h>
#include <stdint.h>
/*
 * @brief initializes a list structure
 * @param p_destroy a function used to destroy the data in a list node
 * @param p_compare a function used to compare the data in each node
 * @return NULL on error else an allocated list structure
 */
list * list_init(void * p_destroy(const void * p_data), 
                 void * p_compare(const void * p_data1, const void * p_data2));

/*
 * @brief adds data to the end of a list structure
 * @param p_list the list to append the data to
 * @param p_data the data to append to the list
 * @return NULL on error else a pointer to the list node that holds the data
 */
list_node * list_append(list * p_list, const void * p_data);

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
void list_destroy(list * p_list);