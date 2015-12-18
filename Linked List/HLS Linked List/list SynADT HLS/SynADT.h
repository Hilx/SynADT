#ifndef PROJECNAME_HEADER_
#define PROJECNAME_HEADER_

#include "SynADT_malloc_wrapper.h"

/*
// included in SynADT_malloc_wrapper.h
typedef volatile int data_t;     // data
*/

typedef volatile int ptr_t;      // pointer
typedef volatile signed next_t;  // offset

#define NULL_PTR 0
#define LIST_DATA_OFFSET 1
#define LIST_NEXT_OFFSET 0

struct list_node_t{
	data_t data;
	next_t next;
};

#define REQ_NODE_SIZE 12
/* List */
/* Memory Node Access - Write Node */
volatile void list_node_write_next(data_t *Master2Mem, int nodePtr, next_t offset);
volatile void list_node_write_data(data_t *Master2Mem, int nodePtr, data_t data);
volatile void list_node_write(data_t *Master2Mem, int nodePtr, struct list_node_t currentNode);
/* Memory Node Access - Read Node */
next_t list_node_read_next(data_t *Master2Mem, int nodePtr);
data_t list_node_read_data(data_t *Master2Mem, int nodePtr);
struct list_node_t node_read(data_t *Master2Mem, int nodePtr);
/* Low-level list manipulation functions */
void list_node_set_next(data_t *Master2Mem, int currentNodePtr, int nextNodePtr);
ptr_t list_node_alloc_new(data_t *Master2Mem, data_t *Master2SysAlloc, data_t data, ptr_t nextNodePtr);
//ptr_t list_node_get_nextNodePtr(data_t *Master2Mem, ptr_t currentNodePtr);
void list_node_delete(data_t *Master2SysAlloc, ptr_t nodePtr);

/* Software C level list manipulation functions */
/* Creating a list */
ptr_t Add2List(data_t *Master2Mem, data_t *Master2SysAlloc, data_t data, ptr_t hdPtr);
/* Reverse the list */
ptr_t ReverseList(data_t *Master2Mem, ptr_t hdPtr);
/* Delete the list */
ptr_t DeleteList(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdPtr);

#endif
