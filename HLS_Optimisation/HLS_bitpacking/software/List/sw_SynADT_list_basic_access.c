#include "sw_SynADT.h"
#include "bitpacking.h"
/* Allocate New Node */
int *list_node_alloc_new(data_t data, int *hdPtr){
	int *newNodePtr = SysMalloc(sizeof(struct node_t));
	
	struct node_t_std newNode;
	newNode.data = data;
	newNode.nextPtr = hdPtr;
	list_node_write_std(newNodePtr, newNode);
	
	return newNodePtr;
}
void list_node_write_std(int *nodePtr, struct node_t_std nowNode){
	struct node_t nodeOut;
	nodeOut.data = nowNode.data;
	signed offset;
	if(nowNode.nextPtr == NULL){
		offset = 0;
	}else{
		offset = ((nowNode.nextPtr - nodePtr)) >> OffsetFactorShift;
	}	
	nodeOut.next = offset;
	list_node_write(nodePtr, nodeOut);
}
void list_node_write(int *nodePtr, struct node_t nowNode){
	int data32pack;
	data32pack = (((unsigned)nowNode.next) << ShiftStep) + nowNode.data;
	nodePtr[0] = data32pack;
}

struct node_t_std list_node_read_std(int *nodePtr){
	struct node_t node_intermediate = list_node_read(nodePtr);
	struct node_t_std nodeOut;
	nodeOut.data = node_intermediate.data;
	if(node_intermediate.next == 0){
		nodeOut.nextPtr = NULL;
	}else{
		nodeOut.nextPtr = nodePtr + ((node_intermediate.next) << OffsetFactorShift);
	}	
	return nodeOut;
}
struct node_t list_node_read(int *nodePtr){
	int data32pack = nodePtr[0];
	struct node_t nowNode;
	nowNode.data = data32pack & KeyMask;
	nowNode.next = (signed)data32pack >> ShiftStep;
	return nowNode;
}