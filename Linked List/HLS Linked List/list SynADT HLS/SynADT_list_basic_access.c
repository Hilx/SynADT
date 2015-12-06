#include "SynADT.h"

/* Memory Node Access - Write Node */
volatile void list_node_write_next(data_t *Master2Mem, ptr_t nodePtr, next_t offset){
	Master2Mem[nodePtr/4 + LIST_NEXT_OFFSET] = offset;
}
volatile void list_node_write_data(data_t *Master2Mem, ptr_t nodePtr, data_t data){
	Master2Mem[nodePtr/4 + LIST_DATA_OFFSET] = data;
}
volatile void list_node_write(data_t *Master2Mem, ptr_t nodePtr, struct list_node_t currentNode){
	list_node_write_data(Master2Mem, nodePtr, currentNode.data);
	list_node_write_next(Master2Mem, nodePtr, currentNode.next);
}

/* Memory Node Access - Read Node */
next_t list_node_read_next(data_t *Master2Mem, ptr_t nodePtr){
	return Master2Mem[nodePtr/4 + LIST_NEXT_OFFSET];

}
data_t list_node_read_data(data_t *Master2Mem, ptr_t nodePtr){
	return Master2Mem[nodePtr/4 + LIST_DATA_OFFSET];
}
struct list_node_t list_node_read(data_t *Master2Mem, ptr_t nodePtr){
	struct list_node_t readNode;
	readNode.data = list_node_read_data(Master2Mem, nodePtr);
	readNode.next = list_node_read_next(Master2Mem, nodePtr);
	return readNode;
}

/* Low-level list manipulation functions */
void list_node_set_next(data_t *Master2Mem, ptr_t currentNodePtr, ptr_t nextNodePtr){
	next_t offset;
	offset = nextNodePtr - currentNodePtr;
	list_node_write_next(Master2Mem, currentNodePtr, offset);
}

ptr_t list_node_alloc_new(data_t *Master2Mem, data_t *Master2SysAlloc, data_t data, ptr_t nextNodePtr){
	ptr_t newNodePtr;
	struct list_node_t newNode;
	newNodePtr = SysMalloc(sizeof(struct list_node_t), Master2SysAlloc);
	newNode.data = data;
	newNode.next = nextNodePtr - newNodePtr;
	list_node_write(Master2Mem, newNodePtr, newNode);
	return newNodePtr;
}
/*
ptr_t list_node_get_nextNodePtr(data_t *Master2Mem, ptr_t currentNodePtr){
	next_t offset = list_node_read_next(Master2Mem, currentNodePtr);
	return currentNodePtr + offset;
}
*/
void list_node_delete(data_t *Master2SysAlloc, ptr_t nodePtr){
	SysFree(nodePtr, Master2SysAlloc);
}
