#include "SynADT.h"
#include "bitpacking.h"
ptr_t list_node_alloc_new(data_t *Master2Mem, data_t *Master2SysAlloc, data_t data, ptr_t nextNodePtr, int *buffPtr){
	ptr_t newNodePtr = SysMalloc(REQ_NODE_SIZE, Master2SysAlloc);
	struct list_node_t_std newNode;
	newNode.data = data;
	newNode.nextPtr = nextNodePtr;
	list_node_write_std(Master2Mem, newNodePtr, newNode, buffPtr);
	return newNodePtr;
}

void list_node_delete(data_t *Master2SysAlloc, ptr_t nodePtr){
	SysFree(nodePtr, Master2SysAlloc);
}

void list_node_write_std(data_t *Master2Mem, ptr_t nodePtr, struct list_node_t_std currentNode, int *buffPtr){
	struct list_node_t nodeOut;
	nodeOut.data = currentNode.data;
	signed offset;
	if(currentNode.nextPtr == NULL_PTR){
		offset = 0;
	}else{
		offset = (currentNode.nextPtr - nodePtr) >> OffsetFactorShift;
	}	
	nodeOut.next = offset;
	list_node_write(Master2Mem, nodePtr, nodeOut, buffPtr);
}
void list_node_write(data_t *Master2Mem, ptr_t nodePtr, struct list_node_t currentNode, int *buffPtr){
	int data32pack;
	data32pack = (((unsigned)currentNode.next) << ShiftStep) + currentNode.data;
	int MemAddr = (unsigned)nodePtr >> 2;
	Master2Mem[MemAddr] = data32pack;
}
struct list_node_t_std list_node_read_std(data_t *Master2Mem, ptr_t nodePtr, int *buffPtr){
	struct list_node_t node_intermediate = list_node_read(Master2Mem, nodePtr, buffPtr);
	struct list_node_t_std nodeOut;
	nodeOut.data = node_intermediate.data;
	if(node_intermediate.next == 0){
		nodeOut.nextPtr = NULL_PTR;
	}else{
		nodeOut.nextPtr = nodePtr + (node_intermediate.next << OffsetFactorShift);
	}
	return nodeOut;
}
struct list_node_t list_node_read(data_t *Master2Mem, ptr_t nodePtr, int *buffPtr){
	int MemAddr = (unsigned)nodePtr >> 2;
	int data32pack = Master2Mem[MemAddr];
	struct list_node_t readNode;
	readNode.data = data32pack & KeyMask;
	readNode.next = (signed)data32pack >> ShiftStep;
	return readNode;
}