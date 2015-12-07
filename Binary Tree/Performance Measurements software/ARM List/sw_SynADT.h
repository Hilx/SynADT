#include <stdio.h>
#include <stdlib.h>
#include "sw_SynADT_malloc_wrapper.h"

#define NULL_PTR 0
#define LEFT_OFFSET 0
#define RIGHT_OFFSET 1
#define DATA_OFFSET 2

typedef signed next_t;
typedef int data_t;

struct node_t{
	next_t left;  // 0
	next_t right; // 1
	data_t data;  // 2
};

struct node_t_std{
	int *leftPtr;  // 0
	int *rightPtr; // 1
	data_t data;   // 2
};

/* Check then Insert */
int *Check_thenInsert(int *root, data_t key);
/* Update */
int *UpdateNode(int *root, int oldKey, int newKey);
/* Insert */
int *Insert(int data, int *tree);
/* Search */
int *Search(int data, int *tree);

/* Delete Node */
int *DeleteTreeNode(int *root, int key);
int *minValueNode(int *node);

/* Delete Tree */
int *DeleteTree(int *tree);

/* Basic Access Functions       */
/* used to access node elements */
/* using offset based method    */

/* Allocate New Node */
int *tree_node_alloc_new(data_t data);

/* Set Pointers */
void tree_set_left_pointer(int *nowNodePtr, int *nextNodePtr);
void tree_set_right_pointer(int *nowNodePtr, int *nextNodePtr);

/* Get Pointers*/
int *tree_get_left_pointer(int *tree);
int *tree_get_right_pointer(int *tree);

/* Standard Whole Node Accesses (with pointers instead of offsets) */
void tree_node_write_std(int *nodePtr, struct node_t_std nowNode);
struct node_t_std tree_node_read_std(int *nodePtr);

/* Whole-node Accesses (with offsets) */
void tree_node_write(int *nodePtr, struct node_t nowNode);
struct node_t tree_node_read(int *nodePtr);

/* Memory Accesses */
/* Writes */
void tree_node_write_left(int *nodePtr, next_t offset);
void tree_node_write_right(int *nodePtr, next_t offset);
void tree_node_write_data(int *nodePtr, data_t data);
/* Reads */
next_t tree_node_read_left(int *nodePtr);
next_t tree_node_read_right(int *nodePtr);
data_t tree_node_read_data(int *nodePtr);