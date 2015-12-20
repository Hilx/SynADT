#include "SynADT.h"

/* Node Operations */
void node_set_left(int *myHeap, ptr_t currentPtr, ptr_t nextPtr){
	next_t offset =  nextPtr - currentPtr;
	node_write_left(myHeap, currentPtr, offset);
}
void node_set_right(int *myHeap, ptr_t currentPtr, ptr_t nextPtr){
	next_t offset =  nextPtr - currentPtr;
	node_write_right(myHeap, currentPtr, offset);
}
void node_set_height(int *myHeap, ptr_t currentPtr, int height){
	node_write_height(myHeap, currentPtr, height);
}

ptr_t node_alloc_new(int *myHeap, data_t data, ptr_t leftNodePtr, ptr_t rightNodePtr){
	ptr_t newNodePtr;
	struct node_t newNode;
	newNodePtr = SysMalloc(sizeof(struct node_t));
	newNode.data = data;
	newNode.left = leftNodePtr - newNodePtr;
	newNode.right = rightNodePtr - newNodePtr;
	newNode.height = 0;
	node_write(myHeap, newNodePtr, newNode);
	return newNodePtr;
}
ptr_t node_get_leftNodePtr(int *myHeap, ptr_t currentNodePtr){
	next_t offset = node_read_left(myHeap, currentNodePtr);
	return currentNodePtr + offset;
}
ptr_t node_get_rightNodePtr(int *myHeap, ptr_t currentNodePtr){
	next_t offset = node_read_right(myHeap, currentNodePtr);
	return currentNodePtr + offset;
}
void node_delete(ptr_t nodePtr){
	SysFree(nodePtr);
}

/* Whole Node Access in Memory */
void node_write(int *myHeap, ptr_t nodePtr, struct node_t nodeIn){
	node_write_data(myHeap, nodePtr, nodeIn.data);
	node_write_left(myHeap, nodePtr, nodeIn.left);
	node_write_right(myHeap, nodePtr, nodeIn.right);
	node_write_height(myHeap, nodePtr, nodeIn.height);
}
struct node_t node_read(int *myHeap, ptr_t nodePtr){
	struct node_t NodeOut;
	NodeOut.data = node_read_data(myHeap, nodePtr);
	NodeOut.left = node_read_left(myHeap, nodePtr);
	NodeOut.right= node_read_right(myHeap, nodePtr);
	NodeOut.height = node_read_height(myHeap, nodePtr);
	return NodeOut;
}

/* Access Node in Memory (SINGLE ELEMENT) - Write Node */
void node_write_left(int *myHeap, ptr_t nodePtr, next_t offset){
	myHeap[nodePtr+LEFT_OFFSET] = offset;	
}
void node_write_right(int *myHeap, ptr_t nodePtr, next_t offset){
	myHeap[nodePtr+RIGHT_OFFSET] = offset;
}
void node_write_data(int *myHeap, ptr_t nodePtr, data_t data){
	myHeap[nodePtr+DATA_OFFSET] = data;
}
void node_write_height(int *myHeap, ptr_t nodePtr, int height){
	myHeap[nodePtr+HEIGHT_OFFSET] = height;
}

/* Access Node in Memory (SINGLE ELEMENT) - Read Node */
next_t node_read_left(int *myHeap, ptr_t nodePtr){
	return myHeap[nodePtr + LEFT_OFFSET];
}
next_t node_read_right(int *myHeap, ptr_t nodePtr){
	return myHeap[nodePtr + RIGHT_OFFSET];
}
data_t node_read_data(int *myHeap, ptr_t nodePtr){
	return myHeap[nodePtr + DATA_OFFSET];
}
int node_read_height(int *myHeap, ptr_t nodePtr){
	return myHeap[nodePtr + HEIGHT_OFFSET];
}