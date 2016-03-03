#include "SynADT.h"

/* Node Operations */
void node_set_left(data_t *myHeap, ptr_t currentPtr, ptr_t nextPtr){
	next_t offset =  nextPtr - currentPtr;
	node_write_left(myHeap, currentPtr, offset);
}
void node_set_right(data_t *myHeap, ptr_t currentPtr, ptr_t nextPtr){
	next_t offset =  nextPtr - currentPtr;
	node_write_right(myHeap, currentPtr, offset);
}
volatile void node_set_height(data_t *myHeap, ptr_t currentPtr, int height){
	node_write_height(myHeap, currentPtr, height);
}

ptr_t node_alloc_new(data_t *myHeap, data_t *Master2SysAlloc, data_t data){
	ptr_t newNodePtr;
	struct node_t newNode;
	newNodePtr = SysMalloc(28, Master2SysAlloc);
	newNode.data = data;
	newNode.left = NULL_PTR - newNodePtr;
	newNode.right = NULL_PTR - newNodePtr;
	newNode.height = 1;
	node_write(myHeap, newNodePtr, newNode);
	return newNodePtr;
}
ptr_t node_get_left_pointer(data_t *myHeap, ptr_t currentNodePtr){
	next_t offset = node_read_left(myHeap, currentNodePtr);
	return currentNodePtr + offset;
}
ptr_t node_get_right_pointer(data_t *myHeap, ptr_t currentNodePtr){
	next_t offset = node_read_right(myHeap, currentNodePtr);
	return currentNodePtr + offset;
}
void node_delete(data_t *Master2SysAlloc, ptr_t nodePtr){
	SysFree(nodePtr, Master2SysAlloc);
}
/* Whole Node Access in Memory */
void node_write_std(data_t *myHeap, ptr_t nodePtr, struct node_t_std nodeIn){
	node_write_data(myHeap, nodePtr, nodeIn.data);
	node_set_left(myHeap, nodePtr, nodeIn.leftPtr);
	node_set_right(myHeap, nodePtr, nodeIn.rightPtr);
	node_write_height(myHeap, nodePtr, nodeIn.height);
}
struct node_t_std node_read_std(data_t *myHeap, ptr_t nodePtr){
	struct node_t_std NodeOut;
	NodeOut.data = node_read_data(myHeap, nodePtr);
	NodeOut.leftPtr = node_get_left_pointer(myHeap, nodePtr);
	NodeOut.rightPtr = node_get_right_pointer(myHeap, nodePtr);
	NodeOut.height = node_read_height(myHeap, nodePtr);
	return NodeOut;
}

/* Whole Node Access in Memory */
void node_write(data_t *myHeap, ptr_t nodePtr, struct node_t nodeIn){
	node_write_data(myHeap, nodePtr, nodeIn.data);
	node_write_left(myHeap, nodePtr, nodeIn.left);
	node_write_right(myHeap, nodePtr, nodeIn.right);
	node_write_height(myHeap, nodePtr, nodeIn.height);
}
struct node_t node_read(data_t *myHeap, ptr_t nodePtr){
	struct node_t NodeOut;
	NodeOut.data = node_read_data(myHeap, nodePtr);
	NodeOut.left = node_read_left(myHeap, nodePtr);
	NodeOut.right= node_read_right(myHeap, nodePtr);
	NodeOut.height = node_read_height(myHeap, nodePtr);
	return NodeOut;
}

/* Access Node in Memory (SINGLE ELEMENT) - Write Node */
void node_write_left(data_t *myHeap, ptr_t nodePtr, next_t offset){
	myHeap[nodePtr/4+LEFT_OFFSET] = offset;	
}
void node_write_right(data_t *myHeap, ptr_t nodePtr, next_t offset){
	myHeap[nodePtr/4+RIGHT_OFFSET] = offset;
}
void node_write_data(data_t *myHeap, ptr_t nodePtr, data_t data){
	myHeap[nodePtr/4+DATA_OFFSET] = data;
}
void node_write_height(data_t *myHeap, ptr_t nodePtr, int height){
	myHeap[nodePtr/4+HEIGHT_OFFSET] = height;
}

/* Access Node in Memory (SINGLE ELEMENT) - Read Node */
next_t node_read_left(data_t *myHeap, ptr_t nodePtr){
	return myHeap[nodePtr/4 + LEFT_OFFSET];
}
next_t node_read_right(data_t *myHeap, ptr_t nodePtr){
	return myHeap[nodePtr/4 + RIGHT_OFFSET];
}
data_t node_read_data(data_t *myHeap, ptr_t nodePtr){
	return myHeap[nodePtr/4 + DATA_OFFSET];
}
int node_read_height(data_t *myHeap, ptr_t nodePtr){
	return myHeap[nodePtr/4 + HEIGHT_OFFSET];
}
