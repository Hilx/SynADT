#include <stdio.h>
#include <stdlib.h>

#include "SynADT_malloc_wrapper.h"

#define NULL_PTR 0

typedef int ptr_t;
typedef signed next_t;

struct node_t{
	signed left; 
	signed right;	
	int height;
	int data; 
};

struct node_t_std{
	ptr_t leftPtr; 
	ptr_t rightPtr;	
	int height;
	int data; 
};

#define DATA_OFFSET 3
#define LEFT_OFFSET 0
#define RIGHT_OFFSET 1
#define HEIGHT_OFFSET 2

/* INSERT HELPER STRUCT */
struct sub_t{
	int feedback;
	ptr_t pointer;
	int flag_same_key;
};

struct search_t{
	ptr_t nodePtr;
	ptr_t parentPtr;
	int stackPtr_avl;
	int flag_failed;
};

struct ptrBundle_t{
	ptr_t root;
	ptr_t nodePtr;
};

/* myStack */
#define READ_STACK 0
#define WRITE_STACK 1
#define GOING_LEFT 0
#define GOING_RIGHT 1

struct stack_t{
	int hdPtr_avl;
	ptr_t pointer;
	int operation;
};


/* Application Level */
/* sub() stuff */
#define FB_DONE 1
#define FB_LEFT 2
#define FB_RIGHT 3

volatile struct stack_t AVL_STACK_WRITE(int *myStack, int hdStack, ptr_t pointer, int operation);
volatile struct stack_t AVL_STACK_READ(int *myStack, int hdStack);

/* AVL tree stuff */
int max(int a, int b);
int CalculateNodeHeight(data_t *myHeap, ptr_t nodePtr);
int height(data_t *myHeap, ptr_t nodePtr);
ptr_t RightRotate(data_t *myHeap, ptr_t yPtr);
ptr_t LeftRotate(data_t *myHeap, ptr_t xPtr);
volatile void node_set_height(data_t *myHeap, ptr_t currentPtr, int height);
void node_write_height(data_t *myHeap, ptr_t nodePtr, int height);
int node_read_height(data_t *myHeap, ptr_t nodePtr);
signed GetBalance(data_t *myHeap, ptr_t nodePtr);
ptr_t ProcessNodeInsertion(data_t *myHeap, ptr_t nowPtr, int key);
ptr_t ProcessNodeDeletion(data_t *myHeap, ptr_t nowPtr);

struct stack_t avlStack(int *myStack, int hdPtr_avl, volatile int command, ptr_t pointer, volatile int operation);

/* Check then Insert */
ptr_t Check_thenInsert(data_t *myHeap, data_t *Master2SysAlloc,  int *fixedStack, int stackPtr_avl, ptr_t rootPtr, int key);
/* Update */
ptr_t UpdateNode(data_t *myHeap, data_t *Master2SysAlloc, int *fixedStack, int *secondStack, int stackPtr_avl, ptr_t rootPtr, int oldKey, int newKey);
/* Delete Node */
ptr_t DeleteTreeNode(data_t *myHeap, data_t *Master2SysAlloc, int *fixedStack, int *secondStack, int stackPtr_avl, ptr_t rootPtr, int key);
struct sub_t DeleteNodeSub(data_t *myHeap, ptr_t nodePtr, int key);
ptr_t minValueNode(data_t *myHeap, ptr_t nodePtr);
ptr_t DeleteSuccessor(data_t *myHeap, data_t *Master2SysAlloc, int *fixedStack, int stackPtr_avl, ptr_t rootPtr, int key);
struct sub_t DeleteSuccessorSub(data_t *myHeap, ptr_t nodePtr, int key);

/* Delete */
ptr_t DeleteTree(data_t *myHeap, data_t *Master2SysAlloc, int *fixedStack, int stackPtr_avl, ptr_t treePtr);
struct sub_t DeleteTreeSub(data_t *myHeap, data_t *Master2SysAlloc, ptr_t treePtr);

/* Search */
struct search_t Search(data_t *myHeap, int *fixedStack, int stackPtr_avl, ptr_t treePtr, int data);
struct sub_t SearchSub(data_t *myHeap, ptr_t treePtr, int data);

/* Create */
struct ptrBundle_t Insert(data_t *myHeap, data_t *Master2SysAlloc, int *fixedStack, int stackPtr_avl, ptr_t treePtr, int data);
struct sub_t InsertSub(data_t *myHeap, data_t *Master2SysAlloc, ptr_t treePtr, int data);

/* Node Operations */
void node_set_left(data_t *myHeap, ptr_t currentPtr, ptr_t nextPtr);
void node_set_right(data_t *myHeap, ptr_t currentPtr, ptr_t nextPtr);
ptr_t node_alloc_new(data_t *myHeap, data_t *Master2SysAlloc, data_t data);
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
/* Access Node in Memory (SINGLE ELEMENT) - Read Node */
next_t node_read_left(data_t *myHeap, ptr_t nodePtr);
next_t node_read_right(data_t *myHeap, ptr_t nodePtr);
data_t node_read_data(data_t *myHeap, ptr_t nodePtr);
