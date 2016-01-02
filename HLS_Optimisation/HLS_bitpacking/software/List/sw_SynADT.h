#include <stdio.h>
#include <stdlib.h>
#include "sw_SynADT_malloc_wrapper.h"

#define NULL_PTR 0
#define NEXT_OFFSET 0
#define DATA_OFFSET 1

typedef signed next_t;
typedef int data_t;

struct node_t{
	next_t next;
	data_t data;  
};

struct node_t_std{
	int *nextPtr;
	data_t data; 
};

int *Add2List(int data, int *hdPtr);
int *ReverseList(int *hdPtr);
int *DeleteList(int *hdPtr);

/* Allocate New Node */
int *list_node_alloc_new(data_t data, int *hdPtr);
void list_node_write_std(int *nodePtr, struct node_t_std nowNode);
void list_node_write(int *nodePtr, struct node_t nowNode);
struct node_t_std list_node_read_std(int *nodePtr);
struct node_t list_node_read(int *nodePtr);
