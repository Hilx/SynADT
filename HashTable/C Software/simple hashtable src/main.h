#include <stdio.h>
#include <stdlib.h>

#define HEAP_SIZE 100 // maximum number of nodes
#define HASH_TABLE_SIZE 10

#define SEARCH_NOT_FOUND 0xFFFFFFFF

typedef int ptr_t;
typedef int entry_index_t;
typedef unsigned next_t; //offset
typedef int data_t;

/* linked list */
#define WORDS_PER_NODE 2
#define NULL_PTR 100

struct list_node_t{
	data_t data;
	next_t next;
};

void list_node_write_next(int *myHeap,ptr_t nodePtr, signed next);
void list_node_write_data(int *myHeap,ptr_t nodePtr, int key);
void list_node_write(int *myHeap, ptr_t nodePtr, struct list_node_t currentNode);
int list_node_read_next(int *myHeap, ptr_t nodePtr);
int list_node_read_data(int *myHeap, ptr_t nodePtr);
struct list_node_t list_node_read(int *myHeap, ptr_t nodePtr);
void list_node_set_next(int *myHeap, ptr_t currentNodePtr, ptr_t nextNodePtr);
ptr_t list_node_alloc_new(int *myHeap, int data, ptr_t nextNodePtr);
ptr_t list_node_get_next_pointer(int *myHeap, ptr_t currentNodePtr);
void list_node_delete(int *myHeap, ptr_t nodePtr);

ptr_t ListGen(int *myHeap,int SizeOfList, ptr_t hdPtr); 
ptr_t Add2List(int *myHeap, int data, ptr_t hdPtr);
ptr_t DeleteListNode_HT(int *myHeap, ptr_t hdPtr, int key);
ptr_t SearchInList(int *myHeap, ptr_t hdPtr, int key);
void PrintList(int *myHeap, ptr_t hdPtr);

/* SysAlloc */
int myHeapIndex;
int SysMalloc(int size);
void SysFree(int addr);

/* hashtable */
ptr_t ReadHashTable(int *myHeap, ptr_t hdTable, ptr_t entryIndex);
void WriteHashTable(int *myHeap, ptr_t hdTable, ptr_t entryIndex, ptr_t listPtr);
ptr_t CreateHashTable(int *myHeap, int size);
entry_index_t HashingAlgorithm(int key);
void InsertNodeHashTable(int *myHeap, ptr_t hdTable, int key);
void DeleteNodeHashTable(int *myHeap, ptr_t hdTable, int key);
ptr_t SearchHashTable(int *myHeap, ptr_t hdTable, int key);