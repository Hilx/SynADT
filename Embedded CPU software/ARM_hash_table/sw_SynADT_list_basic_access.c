#include "sw_SynADT.h"

/* Allocate New Node */
int *list_node_alloc_new(int data, int *hdPtr){
	int *newNodePtr;
	newNodePtr = SysMalloc(NODE_REQ_SIZE);
	
	next_t offset;
	if(hdPtr == NULL){
		offset = NULL_PTR -(int)newNodePtr;
	}else{
		offset = hdPtr - newNodePtr;
	}	

	struct node_t newNode;
	newNode.data = data;
	newNode.next = offset;
	list_node_write(newNodePtr, newNode);
	
	return newNodePtr;
}

void list_node_delete(int *nodePtr){
	SysFree(nodePtr);
}

/* Set Pointers */
void list_set_next_pointer(int *nowNodePtr, int *nextNodePtr){
	next_t offset;
	if(nextNodePtr == NULL){
		offset = NULL_PTR - (int)nowNodePtr;
	}else{
		offset = nextNodePtr - nowNodePtr;
	}	
	list_node_write_next(nowNodePtr, offset);
}

/* Get Pointers*/
int *list_get_next_pointer(int *list){
	int *returnPtr;
	int pointerValue;
	next_t offset = list_node_read_next(list);

	pointerValue = (int)list + offset;
	if(pointerValue == NULL_PTR){
		returnPtr = NULL;
	}else{
		returnPtr = list + offset;
	}
	return returnPtr;
}

/* Standard Whole Node Accesses (with pointers instead of offsets) */
void list_node_write_std(int *nodePtr, struct node_t_std nowNode){
	list_set_next_pointer(nodePtr, nowNode.nextPtr);
	list_node_write_data(nodePtr, nowNode.data);
}

struct node_t_std list_node_read_std(int *nodePtr){
	struct node_t_std nowNode;
	nowNode.nextPtr = list_get_next_pointer(nodePtr);
	nowNode.data = list_node_read_data(nodePtr);
	return nowNode;
}

/* Whole-node Accesses */
void list_node_write(int *nodePtr, struct node_t nowNode){
	list_node_write_next(nodePtr, nowNode.next);
	list_node_write_data(nodePtr, nowNode.data);
}
struct node_t list_node_read(int *nodePtr){
	struct node_t nowNode;
	nowNode.next = list_node_read_next(nodePtr);
	nowNode.data = list_node_read_data(nodePtr);
	return nowNode;
}

/* Writes */
void list_node_write_next(int *nodePtr, next_t offset){
	nodePtr[NEXT_OFFSET] = offset;
}
void list_node_write_data(int *nodePtr, data_t data){
	nodePtr[DATA_OFFSET] = data;
}
/* Reads */
next_t list_node_read_next(int *nodePtr){
	return nodePtr[NEXT_OFFSET];
}
int list_node_read_data(int *nodePtr){
	return nodePtr[DATA_OFFSET];
}
