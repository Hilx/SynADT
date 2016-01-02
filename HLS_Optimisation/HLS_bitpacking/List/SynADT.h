#ifndef PROJECNAME_HEADER_
#define PROJECNAME_HEADER_

#include "SynADT_malloc_wrapper.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef volatile int ptr_t;    
typedef volatile signed next_t;

#define NULL_PTR 0
#define LIST_DATA_OFFSET 1
#define LIST_NEXT_OFFSET 0

struct list_node_t{
	data_t data;
	next_t next;
};

struct list_node_t_std{
	data_t data;
	ptr_t nextPtr;
};

#define REQ_NODE_SIZE 12
/* List */
ptr_t list_node_alloc_new(data_t *Master2Mem, data_t *Master2SysAlloc, data_t data, ptr_t nextNodePtr, int *buffPtr);
void list_node_delete(data_t *Master2SysAlloc, ptr_t nodePtr);

volatile void list_node_write(data_t *Master2Mem, int nodePtr, struct list_node_t currentNode, int *buffPtr);
void list_node_write_std(data_t *Master2Mem, ptr_t nodePtr, struct list_node_t_std currentNode, int *buffPtr);
struct list_node_t list_node_read(data_t *Master2Mem, int nodePtr, int *buffPtr);
struct list_node_t_std list_node_read_std(data_t *Master2Mem, ptr_t nodePtr, int *buffPtr);



/* Software C level list manipulation functions */
ptr_t Add2List(data_t *Master2Mem, data_t *Master2SysAlloc, data_t data, ptr_t hdPtr, int *buffPtr);
ptr_t ReverseList(data_t *Master2Mem, ptr_t hdPtr);
ptr_t DeleteList(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdPtr);

#endif