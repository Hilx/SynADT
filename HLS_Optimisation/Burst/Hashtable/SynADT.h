#ifndef PROJECNAME_HEADER_
#define PROJECNAME_HEADER_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "SynADT_malloc_wrapper.h"

typedef int ptr_t;
typedef signed next_t;
typedef int entry_index_t;

#define NULL_PTR 0
#define SEARCH_NOT_FOUND 0xFFFFFFFF

/* ----------------------- List ----------------------- */
#define LIST_DATA_OFFSET 1
#define LIST_NEXT_OFFSET 0

#define REQ_NODE_SIZE 12

struct list_node_t{
	data_t data;
	next_t next;
};

struct list_node_t_std{
	data_t data;
	ptr_t nextPtr;
};

/* ****** High Level ****** */
ptr_t Add2SortedList(data_t *Master2Mem, data_t *Master2SysAlloc, data_t data, ptr_t hdPtr, int *buffPtr);
ptr_t SearchInList(data_t *Master2Mem, ptr_t hdPtr, int key, int *buffPtr);
ptr_t DeleteListNode_HT(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdPtr, int key, int *buffPtr);
void DeleteList(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdPtr);

/* ****** Basic Access ****** */
/* new node */
ptr_t list_node_alloc_new(data_t *Master2Mem, data_t *Master2SysAlloc, data_t data, ptr_t nextNodePtr, int *buffPtr);
/* delete node */
void list_node_delete(data_t *Master2SysAlloc, ptr_t nodePtr);
/* next pointer */
void list_node_set_next_pointer(data_t *Master2Mem, int currentNodePtr, int nextNodePtr);
ptr_t list_node_get_next_pointer(data_t *Master2Mem, ptr_t currentNodePtr);
/* Standard Whole Node Access (with pointers instead of offsets) */
void list_node_write_std(data_t *Master2Mem, ptr_t nodePtr, struct list_node_t_std currentNode, int *buffPtr);
struct list_node_t_std list_node_read_std(data_t *Master2Mem, ptr_t nodePtr, int *buffPtr);
/* Whole Node Access*/
void list_node_write(data_t *Master2Mem, int nodePtr, struct list_node_t currentNode, int *buffPtr);
struct list_node_t list_node_read(data_t *Master2Mem, int nodePtr,int *buffPtr);
/* Write Node */
void list_node_write_next(data_t *Master2Mem, int nodePtr, next_t offset);
void list_node_write_data(data_t *Master2Mem, int nodePtr, data_t data);
/* Read Node */
next_t list_node_read_next(data_t *Master2Mem, int nodePtr);
data_t list_node_read_data(data_t *Master2Mem, int nodePtr);

/* ----------------------- HashTable -----------------------*/
int HASH_TABLE_SIZE;

void CheckInsert_HashTable(data_t *Master2Mem,  data_t *Master2SysAlloc, ptr_t hdTable, int key,int *buffPtr);
void UpdateNode_HashTable(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdTable, int oldKey, int newKey, int *buffPtr);
ptr_t CreateHashTable(data_t *Master2Mem, data_t *Master2SysAlloc, int size);
void DeleteHashTable(data_t *Master2Mem, data_t *Master2SysAlloc, int size, ptr_t hdTable);
entry_index_t HashingAlgorithm(int key);
void InsertNodeHashTable(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdTable, int key, int *buffPtr);
void DeleteNodeHashTable(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdTable, int key, int *buffPtr);
ptr_t SearchHashTable(data_t *Master2Mem, ptr_t hdTable, int key, int *buffPtr);
ptr_t ReadHashTable(data_t *Master2Mem, ptr_t hdTable, ptr_t entryIndex);
void WriteHashTable(data_t *Master2Mem, ptr_t hdTable, ptr_t entryIndex, ptr_t listPtr);

#endif
