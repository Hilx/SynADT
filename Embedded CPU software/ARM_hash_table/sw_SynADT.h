#include <stdio.h>
#include <stdlib.h>
#include "sw_SynADT_malloc_wrapper.h"

/* ------------------------ Hashtable --------------------------------- */
int HASH_TABLE_SIZE;

void Check_thenInsert(int *hdTable, int key);
void UpdateNode(int *hdTable, int oldKey, int newKey);
int *CreateHashTable(int NUM_OF_ENTRY);
void DeleteHashTable(int NUM_OF_ENTRY, int *hdTable);
int HashingAlgorithm(int key);
void InsertNodeHashTable(int *hdTable, int key);
void DeleteNodeHashTable(int *hdTable, int key);
int *SearchHashTable(int *hdTable, int key);
int *ReadHashTable(int *hdTable, int entryIndex);
void WriteHashTable(int *hdTable, int entryIndex, int *listPtr);


/* software */
void PrintHashTable(int *hdTable);

/* ------------------------ Sorted Linked List ------------------------ */
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

/* high level */
int *Add2SortedList(int data, int *hdPtr);
int *ReverseList(int *hdPtr);
int *DeleteList(int *hdPtr);
int *SearchInList(int *hdList, int key);
int *DeleteListNode_HT(int *hdPtr, int key);

/* Basic Access Functions       */
/* used to access node elements */
/* using offset based method    */

/* Allocate New Node */
int *list_node_alloc_new(data_t data, int *hdPtr);
/* Delete Node */
void list_node_delete(int *nodePtr);
/* Set Pointers */
void list_set_next_pointer(int *nowNodePtr, int *nextNodePtr);
/* Get Pointers*/
int *list_get_next_pointer(int *list);
/* Standard Whole Node Accesses (with pointers instead of offsets) */
void list_node_write_std(int *nodePtr, struct node_t_std nowNode);
struct node_t_std list_node_read_std(int *nodePtr);
/* Whole-node Accesses */
/* Memory Accesses */
void list_node_write(int *nodePtr, struct node_t node);
struct node_t list_node_read(int *nodePtr);
/* Writes */
void list_node_write_next(int *nodePtr, next_t offset);
void list_node_write_data(int *nodePtr, data_t data);
/* Reads */
next_t list_node_read_next(int *nodePtr);
data_t list_node_read_data(int *nodePtr);

/* SW only */
void PrintList(int *hdPtr);
