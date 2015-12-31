#include "SynADT.h"

/* new node */
ptr_t list_node_alloc_new(data_t *Master2Mem, data_t *Master2SysAlloc, data_t data, ptr_t nextNodePtr, int *buffPtr){
	ptr_t newNodePtr;
	struct list_node_t newNode;
	newNodePtr = SysMalloc(REQ_NODE_SIZE, Master2SysAlloc);
	newNode.data = data;
	newNode.next = nextNodePtr - newNodePtr;
	list_node_write(Master2Mem, newNodePtr, newNode, buffPtr);
	return newNodePtr;
}
/* delete node */
void list_node_delete(data_t *Master2SysAlloc, ptr_t nodePtr){
	SysFree(nodePtr, Master2SysAlloc);
}
/* next pointer */
void list_node_set_next_pointer(data_t *Master2Mem, ptr_t currentNodePtr, ptr_t nextNodePtr){
	next_t offset;
	offset = nextNodePtr - currentNodePtr;
	list_node_write_next(Master2Mem, currentNodePtr, offset);
}
ptr_t list_node_get_next_pointer(data_t *Master2Mem, ptr_t currentNodePtr){
	next_t offset = list_node_read_next(Master2Mem, currentNodePtr);
	return currentNodePtr + offset;
}

/* Whole Node */
void list_node_write(data_t *Master2Mem, ptr_t nodePtr, struct list_node_t currentNode, int *buffPtr){
	buffPtr[LIST_DATA_OFFSET] = currentNode.data;
	buffPtr[LIST_NEXT_OFFSET] = currentNode.next;
	memcpy((int *)(Master2Mem + nodePtr/4), buffPtr, 2*sizeof(int));
}
void list_node_write_std(data_t *Master2Mem, ptr_t nodePtr, struct list_node_t_std currentNode, int *buffPtr){
	struct list_node_t nodeOut;
	nodeOut.data = currentNode.data;
	nodeOut.next = currentNode.nextPtr - nodePtr;
	list_node_write(Master2Mem, nodePtr, nodeOut, buffPtr);
}
struct list_node_t list_node_read(data_t *Master2Mem, ptr_t nodePtr, int *buffPtr){
	memcpy(buffPtr, (int *)(Master2Mem + nodePtr/4), 2*sizeof(int));
	struct list_node_t readNode;
	readNode.data = buffPtr[LIST_DATA_OFFSET];
	readNode.next = buffPtr[LIST_NEXT_OFFSET];
	return readNode;
}
struct list_node_t_std list_node_read_std(data_t *Master2Mem, ptr_t nodePtr, int *buffPtr){
	struct list_node_t node_intermediate = list_node_read(Master2Mem, nodePtr, buffPtr);
	struct list_node_t_std nodeOut;
	nodeOut.data = node_intermediate.data;
	nodeOut.nextPtr = (next_t)node_intermediate.next + nodePtr;
	return nodeOut;
}
/* Memory Node Access - Write Node */
void list_node_write_next(data_t *Master2Mem, ptr_t nodePtr, next_t offset){
	Master2Mem[nodePtr/4 + LIST_NEXT_OFFSET] = offset;
}
void list_node_write_data(data_t *Master2Mem, ptr_t nodePtr, data_t data){
	Master2Mem[nodePtr/4 + LIST_DATA_OFFSET] = data;
}
/* Memory Node Access - Read Node */
next_t list_node_read_next(data_t *Master2Mem, ptr_t nodePtr){
	return Master2Mem[nodePtr/4 + LIST_NEXT_OFFSET];
}
data_t list_node_read_data(data_t *Master2Mem, ptr_t nodePtr){
	return Master2Mem[nodePtr/4 + LIST_DATA_OFFSET];
}
