#include <stdio.h>
#include <stdlib.h>

struct node_t{
	signed left; //0
	signed right;	//1
	int data; //2
};
#define NODE_SIZE 3 // each node has 3 integers
#define HEAP_SIZE 100 // maximum number of nodes
#define NULL_PTR NODE_SIZE*HEAP_SIZE //NULL pointer has an impossible index value
#define DATA_OFFSET 2
#define LEFT_OFFSET 0
#define RIGHT_OFFSET 1

typedef int ptr_t;
typedef signed next_t;
typedef int data_t;

/* INSERT HELPER STRUCT */
struct insert_t{
	int feedback;
	ptr_t pointer;
};

/* RAM */
int myHeapIndex;

/* myStack */
#define WORDS_PER_NODE 2
#define LIST_NULL_PTR 0
#define READ_STACK 0
#define WRITE_STACK 1
#define GOING_LEFT 0
#define GOING_RIGHT 1

struct stack_t{
	int *hdPtr;
	ptr_t pointer;
	int operation;
};

/* Software Only */
void printTree(int *myHeap, ptr_t nowPtr);
void printValue(int *myHeap, ptr_t nowPtr);

/* Application Level */
#define FB_DONE 1
#define FB_LEFT 2
#define FB_RIGHT 3

ptr_t TreeGen(int*myHeap, int *stackPtr, int NumberOfNodes);
ptr_t Insert(int *myHeap, int *stackPtr, ptr_t treePtr, int data);
struct insert_t InsertSub(int *myHeap, ptr_t treePtr, int data);

/* Node Operations */
void node_set_left(int *myHeap, ptr_t currentPtr, ptr_t nextPtr);
void node_set_right(int *myHeap, ptr_t currentPtr, ptr_t nextPtr);
ptr_t node_alloc_new(int *myHeap, data_t data, ptr_t leftNodePtr, ptr_t rightNodePtr);
ptr_t node_get_leftNodePtr(int *myHeap, ptr_t currentNodePtr);
ptr_t node_get_rightNodePtr(int *myHeap, ptr_t currentNodePtr);
void node_delete(ptr_t nodePtr);
/* Whole Node Access in Memory */
void node_write(int *myHeap, ptr_t nodePtr, struct node_t nodeIn);
struct node_t node_read(int *myHeap, ptr_t nodePtr);
/* Access Node in Memory (SINGLE ELEMENT) - Write Node */
void node_write_left(int *myHeap, ptr_t nodePtr, next_t offset);
void node_write_right(int *myHeap, ptr_t nodePtr, next_t offset);
void node_write_data(int *myHeap, ptr_t nodePtr, data_t data);
/* Access Node in Memory (SINGLE ELEMENT) - Read Node */
next_t node_read_left(int *myHeap, ptr_t nodePtr);
next_t node_read_right(int *myHeap, ptr_t nodePtr);
data_t node_read_data(int *myHeap, ptr_t nodePtr);

/* SYSALLOC FUNCTIONS */
int SysMalloc(int size);
void SysFree(int addr);


int *ListGen(int SizeOfList, int *hdPtr); 
int *Add2List(int data, int *hdPtr);
int *ReverseList(int *hdPtr);
int *DeleteList(int *hdPtr);
int *DeleteNode(int *hdPtr);
void PrintList(int *hdPtr);
struct stack_t myStack(int *hdPtr, int command, ptr_t pointer, int operation);
