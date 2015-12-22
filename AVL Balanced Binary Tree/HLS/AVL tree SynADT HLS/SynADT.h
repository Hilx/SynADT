#include <stdio.h>
#include <stdlib.h>

#include "SynADT_malloc_wrapper.h"

#define NULL_PTR 0
typedef volatile int ptr_t;
typedef volatile signed next_t;
#define DATA_OFFSET 3
#define LEFT_OFFSET 0
#define RIGHT_OFFSET 1
#define HEIGHT_OFFSET 2

#define REQ_NODE_SIZE 28

struct node_t{
	next_t left; 
	next_t right;
	int height;
	data_t data;
};
struct node_t_std{
	ptr_t leftPtr; 
	ptr_t rightPtr;	
	int height;
	int data; 
};

#define FB_DONE 1
#define FB_LEFT 2
#define FB_RIGHT 3

struct sub_t{
	int feedback;
	ptr_t pointer;
	int flag_same_key;
};

struct search_t{
	ptr_t nodePtr;
	ptr_t parentPtr;
	int direction;
	int stackPtr_avl;
	int flag_failed;
};

struct ptrBundle_t{
	ptr_t root;
	ptr_t nodePtr;
};

/* -------- High Level -------- */
// Check Insert
ptr_t Check_thenInsert(data_t *myHeap, data_t *Master2SysAlloc, data_t *fixedStack, ptr_t stackPtr_avl, ptr_t rootPtr, int key);
// Update
ptr_t UpdateNode(data_t *myHeap, data_t *Master2SysAlloc, data_t *fixedStack, data_t *secondStack, ptr_t stackPtr_avl, ptr_t rootPtr, int oldKey, int newKey);
// Insert
struct ptrBundle_t Insert(data_t *myHeap, data_t *Master2SysAlloc, data_t *fixedStack, ptr_t stackPtr_avl, ptr_t treePtr, int data, ptr_t new_leftPtr, ptr_t new_rightPtr);
struct sub_t InsertSub(data_t *myHeap, data_t *Master2SysAlloc, ptr_t treePtr, int data, ptr_t new_leftPtr, ptr_t new_rightPtr);
// Search
struct search_t Search(data_t *myHeap, data_t *fixedStack, ptr_t stackPtr_avl, ptr_t treePtr, int data);
struct sub_t SearchSub(data_t *myHeap, ptr_t treePtr, int data);
// Delete Node
ptr_t DeleteTreeNode(data_t *myHeap, data_t *Master2SysAlloc, data_t *fixedStack, data_t *secondStack, ptr_t stackPtr_avl, ptr_t rootPtr, int key);
struct sub_t DeleteNodeSub(data_t *myHeap, ptr_t nodePtr, int key);
ptr_t minValueNode(data_t *myHeap, ptr_t nodePtr);
ptr_t DeleteSuccessor(data_t *myHeap, data_t *Master2SysAlloc, data_t *fixedStack, ptr_t stackPtr_avl, ptr_t rootPtr, int key);
struct sub_t DeleteSuccessorSub(data_t *myHeap, ptr_t nodePtr, int key);
// Delete Tree
ptr_t DeleteTree(data_t *myHeap,  data_t *Master2SysAlloc, data_t *fixedStack, ptr_t stackPtr_avl, ptr_t treePtr);
struct sub_t DeleteTreeSub(data_t *myHeap, data_t *Master2SysAlloc, ptr_t treePtr);

/* -------- Balancing -------- */
int max(int a, int b);
int CalculateNodeHeight(data_t *myHeap, ptr_t nodePtr);
int height(data_t *myHeap, ptr_t nodePtr);
ptr_t RightRotate(data_t *myHeap, ptr_t yPtr);
ptr_t LeftRotate(data_t *myHeap, ptr_t xPtr);
signed GetBalance(data_t *myHeap, ptr_t nodePtr);
ptr_t ProcessNodeInsertion(data_t *myHeap, ptr_t nowPtr, int key);
ptr_t ProcessNodeDeletion(data_t *myHeap, ptr_t nowPtr);

/* -------- Basic Access -------- */
void node_set_left(data_t *myHeap, ptr_t currentPtr, ptr_t nextPtr);
void node_set_right(data_t *myHeap, ptr_t currentPtr, ptr_t nextPtr);
void node_set_height(data_t *myHeap, ptr_t currentPtr, int height);
ptr_t node_alloc_new(data_t *myHeap, data_t *Master2SysAlloc, data_t data, ptr_t leftNodePtr, ptr_t rightNodePtr);
ptr_t node_get_left_pointer(data_t *myHeap, ptr_t currentNodePtr);
ptr_t node_get_right_pointer(data_t *myHeap, ptr_t currentNodePtr);
void node_delete(data_t *Master2SysAlloc, ptr_t nodePtr);
void node_write_std(data_t *myHeap, ptr_t nodePtr, struct node_t_std nodeIn);
struct node_t_std node_read_std(data_t *myHeap, ptr_t nodePtr);
/* Whole Node Access in Memory */
void node_write(data_t *myHeap, ptr_t nodePtr, struct node_t nodeIn);
struct node_t node_read(data_t *myHeap, ptr_t nodePtr);
/* Access Node in Memory (SINGLE ELEMENT) - Write Node */
void node_write_left(data_t *myHeap, ptr_t nodePtr, next_t offset);
void node_write_right(data_t *myHeap, ptr_t nodePtr, next_t offset);
void node_write_data(data_t *myHeap, ptr_t nodePtr, data_t data);
void node_write_height(data_t *myHeap, ptr_t nodePtr, int height);
/* Access Node in Memory (SINGLE ELEMENT) - Read Node */
next_t node_read_left(data_t *myHeap, ptr_t nodePtr);
next_t node_read_right(data_t *myHeap, ptr_t nodePtr);
data_t node_read_data(data_t *myHeap, ptr_t nodePtr);
int node_read_height(data_t *myHeap, ptr_t nodePtr);

/* -------- Stack -------- */
struct stack_t{
	int hdPtr_avl;
	ptr_t pointer;
	int operation;
};

#define READ_STACK 0
#define WRITE_STACK 1
#define GOING_LEFT 0
#define GOING_RIGHT 1

struct stack_t avlStack(data_t *myStack, ptr_t hdPtr_avl, int command, ptr_t pointer, int operation);