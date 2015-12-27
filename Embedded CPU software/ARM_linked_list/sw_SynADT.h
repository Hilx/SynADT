#include <stdio.h>
#include <stdlib.h>
#include "sw_SynADT_malloc_wrapper.h"

#define NULL_PTR 0
#define NEXT_OFFSET 0
#define DATA_OFFSET 1

typedef signed next_t;
typedef int data_t;

#define NODE_REQ_SIZE 12

struct node_t{
	next_t next;
	data_t data;  
};

struct node_t_std{
	int *nextPtr;
	data_t data; 
};

int *Add2List(int data, int *hdPtr);
int *ReverseList(int *hdPtr);
int *DeleteList(int *hdPtr);

/* Basic Access Functions       */
/* used to access node elements */
/* using offset based method    */

/* Allocate New Node */
int *list_node_alloc_new(data_t data, int *hdPtr);
/* Set Pointers */
void list_set_next_pointer(int *nowNodePtr, int *nextNodePtr);
/* Get Pointers*/
int *list_get_next_pointer(int *list);
/* Standard Whole Node Accesses (with pointers instead of offsets) */
void list_node_write_std(int *nodePtr, struct node_t_std nowNode);
struct node_t_std list_node_read_std(int *nodePtr);
/* Whole-node Accesses */
void list_node_write(int *nodePtr, struct node_t nowNode);
struct node_t list_node_read(int *nodePtr);
/* Memory Accesses */
/* Writes */
void list_node_write_next(int *nodePtr, next_t offset);
void list_node_write_data(int *nodePtr, data_t data);
/* Reads */
next_t list_node_read_next(int *nodePtr);
data_t list_node_read_data(int *nodePtr);
