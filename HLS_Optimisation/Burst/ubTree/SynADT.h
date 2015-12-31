#include "SynADT_malloc_wrapper.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>


typedef volatile int ptr_t;      // pointer
typedef volatile signed next_t;  // offset

#define NULL_PTR 0
#define REQ_NODE_SIZE 12

/* ----------------------- List ----------------------- */
#define LIST_DATA_OFFSET 1
#define LIST_NEXT_OFFSET 0

struct list_node_t{
	data_t data;
	next_t next;
};

/* Base Functions */
/* Memory Node Access - Write Node */
void list_node_write_next(data_t *Master2Mem, int nodePtr, next_t offset);
void list_node_write_data(data_t *Master2Mem, int nodePtr, data_t data);
void list_node_write(data_t *Master2Mem, int nodePtr, struct list_node_t currentNode, int * buffPtr);
/* Memory Node Access - Read Node */
next_t list_node_read_next(data_t *Master2Mem, int nodePtr);
data_t list_node_read_data(data_t *Master2Mem, int nodePtr);
struct list_node_t list_node_read(data_t *Master2Mem, int nodePtr);
/* Low-level list manipulation functions */
void list_node_set_next(data_t *Master2Mem, int currentNodePtr, int nextNodePtr);
ptr_t list_node_alloc_new(data_t *Master2Mem, data_t *Master2SysAlloc, data_t data, ptr_t nextNodePtr, int * buffPtr);
ptr_t list_node_get_next_pointer(data_t *Master2Mem, ptr_t currentNodePtr);
void list_node_delete(data_t *Master2SysAlloc, ptr_t nodePtr);

/* Application Level functions */

ptr_t Add2List(data_t *Master2Mem, data_t *Master2SysAlloc, data_t data, ptr_t hdPtr, int * buffPtr);
ptr_t DeleteListNode(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdPtr);

/* ----------------------- Tree ----------------------- */
#define TREE_DATA_OFFSET 2
#define TREE_LEFT_OFFSET 0
#define TREE_RIGHT_OFFSET 1
#define FB_DONE 1
#define FB_LEFT 2
#define FB_RIGHT 3

struct tree_node_t{
	next_t left;  
	next_t right;	
	int data;       
};

struct tree_node_t_std{
	ptr_t leftPtr; 
	ptr_t rightPtr;	
	int data;       
};

struct pointer_set_t{
	ptr_t leftPtr;
	ptr_t rightPtr;
};

struct sub_t{
	int feedback;
	ptr_t pointer;
	int flag_same_key;
};

struct search_t{
	ptr_t nodePtr;
	ptr_t parentPtr;
	int direction;
	int flag_failed;
};
struct stack_t{
	ptr_t hdPtr;
	ptr_t pointer;
	int operation;
};

/* Base Functions */
/* Memory Node Access - Write Node */
void tree_node_write_left(data_t *Master2Mem, ptr_t nodePtr, next_t offset);
void tree_node_write_right(data_t *Master2Mem, ptr_t nodePtr, next_t offset);
void tree_node_write_data(data_t *Master2Mem, ptr_t nodePtr, data_t data);
void tree_node_write(data_t *Master2Mem, ptr_t nodePtr, struct tree_node_t currentNode, int * buffPtr);
void tree_node_write_std(data_t *Master2Mem, ptr_t nodePtr, struct tree_node_t_std currentNode, int * buffPtr);
/* Memory Node Access - Read Node */
next_t tree_node_read_left(data_t *Master2Mem, ptr_t nodePtr);
next_t tree_node_read_right(data_t *Master2Mem, ptr_t nodePtr);
data_t tree_node_read_data(data_t *Master2Mem, ptr_t nodePtr);
struct tree_node_t tree_node_read(data_t *Master2Mem, ptr_t nodePtr, int * buffPtr);
struct tree_node_t_std tree_node_read_std(data_t *Master2Mem, ptr_t nodePtr, int * buffPtr);
/* Low-level tree manipulation functions */
struct pointer_set_t tree_get_both_pointers(data_t *Master2Mem, ptr_t nodePtr, int * buffPtr);

void tree_node_set_left(data_t *Master2Mem, ptr_t currentNodePtr, ptr_t nextNodePtr);
ptr_t tree_node_alloc_new(data_t *Master2Mem, data_t *Master2SysAlloc, data_t data, ptr_t leftNodePtr, ptr_t rightNodePtr, int * buffPtr);
ptr_t tree_node_get_left_pointer(data_t *Master2Mem, ptr_t currentNodePtr);
ptr_t tree_node_get_right_pointer(data_t *Master2Mem, ptr_t currentNodePtr);
void tree_node_delete(data_t *Master2SysAlloc, ptr_t nodePtr);

/* Application Level functions */
/* Create Tree */
ptr_t Insert(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t stackPtr, ptr_t treePtr, int data, int * buffPtr);
struct sub_t InsertSub(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t treePtr, data_t data, int * buffPtr);
/* Search */
struct search_t Search(data_t *Master2Mem, ptr_t treePtr, int data, int * buffPtr);
struct sub_t SearchSub(data_t *Master2Mem, ptr_t treePtr, int data, int * buffPtr);
/* Delete Tree */
ptr_t DeleteTree(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t stackPtr, ptr_t treePtr, int * buffPtr);
struct sub_t DeleteTreeSub(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t treePtr, int * buffPtr);
/* Delete Node */
ptr_t DeleteTreeNode(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t stackPtr, ptr_t rootPtr, int key, int * buffPtr);

/* Check then Insert */
ptr_t Check_thenInsert(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t stackPtr, ptr_t rootPtr, int key, int * buffPtr);
/* Update Node */
ptr_t UpdateNode(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t stackPtr, ptr_t rootPtr, int oldKey, int newKey, int * buffPtr);

/* Stack */
#define READ_STACK 0
#define WRITE_STACK 1
#define GOING_LEFT 0
#define GOING_RIGHT 1
struct stack_t dynamicStack(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdPtr, int command, ptr_t pointer, int operation, int * buffPtr);
