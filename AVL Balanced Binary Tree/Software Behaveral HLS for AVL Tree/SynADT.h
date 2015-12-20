#include <stdio.h>
#include <stdlib.h>

// malloc
int SysMalloc(int size);
void SysFree(int addr);

//-----------

struct node_t{
	signed left; //0
	signed right;	//1
	int height;//2
	int data; //3
};
#define NODE_SIZE 4 // each node has 3 integers
#define HEAP_SIZE 1000 // maximum number of nodes
#define NULL_PTR NODE_SIZE*HEAP_SIZE //NULL pointer has an impossible index value
#define DATA_OFFSET 3
#define LEFT_OFFSET 0
#define RIGHT_OFFSET 1
#define HEIGHT_OFFSET 2
#define CORNER_HEIGHT 99

typedef int ptr_t;
typedef signed next_t;
typedef int data_t;



/* INSERT HELPER STRUCT */
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
	int used_stack;
};

struct ptrBundle_t{
	ptr_t root;
	ptr_t nodePtr;
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
	int hdPtr_avl;
	ptr_t pointer;
	int operation;
};

/* Application Level */
/* sub() stuff */
#define FB_DONE 1
#define FB_LEFT 2
#define FB_RIGHT 3

/* AVL tree stuff */
int max(int a, int b);
int CalculateNodeHeight(int *myHeap, ptr_t nodePtr);
int height(int *myHeap, ptr_t nodePtr);
ptr_t RightRotate(int *myHeap, ptr_t yPtr);
ptr_t LeftRotate(int *myHeap, ptr_t xPtr);
void node_set_height(int *myHeap, ptr_t currentPtr, int height);
void node_write_height(int *myHeap, ptr_t nodePtr, int height);
int node_read_height(int *myHeap, ptr_t nodePtr);
signed GetBalance(int *myHeap, ptr_t nodePtr);
ptr_t ProcessNodeInsertion(int *myHeap, ptr_t nowPtr, int key);
ptr_t ProcessNodeDeletion(int *myHeap, ptr_t nowPtr);

struct stack_t avlStack(int *myStack, int hdPtr_avl, int command, ptr_t pointer, int operation);

/* Check then Insert */
ptr_t Check_thenInsert(int *myHeap, int *fixedStack, int stackPtr_avl, ptr_t rootPtr, int key);
/* Update */
ptr_t UpdateNode(int *myHeap, int *fixedStack, int stackPtr_avl, ptr_t rootPtr, int oldKey, int newKey);
/* Delete Node */
ptr_t DeleteTreeNode(int *myHeap, int *fixedStack, int stackPtr_avl, ptr_t rootPtr, int key);

/* Delete */
ptr_t DeleteTree(int *myHeap, int *fixedStack, int stackPtr_avl, ptr_t treePtr);
struct sub_t DeleteTreeSub(int *myHeap, ptr_t treePtr);

/* Search */
struct search_t Search(int *myHeap, int *fixedStack, int stackPtr_avl, ptr_t treePtr, int data);
struct sub_t SearchSub(int *myHeap, ptr_t treePtr, int data);

/* Create */
ptr_t TreeGen(int*myHeap,int *fixedStack, int stackPtr_avl, int NumberOfNodes);
struct ptrBundle_t Insert(int *myHeap, int *fixedStack, int stackPtr_avl, ptr_t treePtr, int data, ptr_t new_leftPtr, ptr_t new_rightPtr);
struct sub_t InsertSub(int *myHeap, ptr_t treePtr, int data, ptr_t new_leftPtr, ptr_t new_rightPtr);

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