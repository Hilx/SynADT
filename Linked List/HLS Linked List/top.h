#ifndef TOP_H_
#define TOP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef volatile int data_t;
typedef volatile int ptr_t;
typedef volatile signed next_t;

#define NULL_PTR 0
#define LIST_DATA_OFFSET 1
#define LIST_NEXT_OFFSET 0

struct list_node_t{
	data_t data;
	next_t next;
};
/* counter stuff */
//addresses
#define COUNTER_BASE 0x43C20000 // <- change this according to your system
#define COUNTER_RESULT 4 // only needed in ARM core if you want to read result back to software
//commands
#define C_START 1
#define C_STOP 2
#define C_RESET 3 // probably only needed in ARM core to reset the counter after the result has been read

/* SysAlloc */
#define MMU_BASE 0x43C00000
#define MMU_TOKEN 0
#define MMU_STATUS 4
#define MMU_RESULT 8
#define MMU_CMD 12
#define MMU_FREE 16
volatile int SysMalloc(int size, data_t *Master2SysAlloc);
volatile void SysFree(int addr, data_t *Master2SysAlloc);

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
ptr_t ListGen(data_t *Master2Mem, data_t *Master2SysAlloc, int SizeOfList, ptr_t hdPtr);
ptr_t Add2List(data_t *Master2Mem, data_t *Master2SysAlloc, data_t data, ptr_t hdPtr);
/* Reverse the list */
ptr_t ReverseList(data_t *Master2Mem, ptr_t hdPtr);
/* Delete the list */
ptr_t DeleteList(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdPtr);

#endif
