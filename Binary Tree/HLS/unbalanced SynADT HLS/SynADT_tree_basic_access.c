#include "SynADT.h"

/* Low-level list manipulation functions */
void tree_node_set_left(data_t *Master2Mem, ptr_t currentNodePtr, ptr_t nextNodePtr){
	next_t offset;
	offset = nextNodePtr - currentNodePtr;
	tree_node_write_left(Master2Mem, currentNodePtr, offset);
}
void tree_node_set_right(data_t *Master2Mem, ptr_t currentNodePtr, ptr_t nextNodePtr){
	next_t offset;
	offset = nextNodePtr - currentNodePtr;
	tree_node_write_right(Master2Mem, currentNodePtr, offset);
}
ptr_t tree_node_alloc_new(data_t *Master2Mem, data_t *Master2SysAlloc, data_t data, ptr_t leftNodePtr, ptr_t rightNodePtr){
	ptr_t newNodePtr;
	struct tree_node_t newNode;
	newNodePtr = SysMalloc(sizeof(struct tree_node_t), Master2SysAlloc);
	newNode.data = data;
	newNode.left = leftNodePtr - newNodePtr;
	newNode.right = rightNodePtr - newNodePtr;
	tree_node_write(Master2Mem, newNodePtr, newNode);
	return newNodePtr;
}
ptr_t tree_node_get_left_pointer(data_t *Master2Mem, ptr_t currentNodePtr){
	next_t offset = tree_node_read_left(Master2Mem, currentNodePtr);
	return currentNodePtr + offset;
}
ptr_t tree_node_get_right_pointer(data_t *Master2Mem, ptr_t currentNodePtr){
	next_t offset = tree_node_read_right(Master2Mem, currentNodePtr);
	return currentNodePtr + offset;
}
void tree_node_delete(data_t *Master2SysAlloc, ptr_t nodePtr){
	SysFree(nodePtr, Master2SysAlloc);
}

/* Whole node access */
void tree_node_write(data_t *Master2Mem, ptr_t nodePtr, struct tree_node_t currentNode){
	tree_node_write_data(Master2Mem, nodePtr, currentNode.data);
	tree_node_write_left(Master2Mem, nodePtr, currentNode.left);
	tree_node_write_right(Master2Mem, nodePtr, currentNode.right);
}
struct tree_node_t node_read(data_t *Master2Mem, ptr_t nodePtr){
	struct tree_node_t readNode;
	readNode.data = tree_node_read_data(Master2Mem, nodePtr);
	readNode.left = tree_node_read_next(Master2Mem, nodePtr);
	readNode.right= tree_node_read_next(Master2Mem, nodePtr);
	return readNode;
}

/* Memory Node Access - Write Node */
void tree_node_write_left(data_t *Master2Mem, ptr_t nodePtr, next_t offset){
	Master2Mem[nodePtr/4 + TREE_LEFT_OFFSET] = offset;
}

void tree_node_write_right(data_t *Master2Mem, ptr_t nodePtr, next_t offset){
	Master2Mem[nodePtr/4 + TREE_RIGHT_OFFSET] = offset;
}
void tree_node_write_data(data_t *Master2Mem, ptr_t nodePtr, data_t data){
	Master2Mem[nodePtr/4 + TREE_DATA_OFFSET] = data;
}

/* Memory Node Access - Read Node */
next_t tree_node_read_left(data_t *Master2Mem, ptr_t nodePtr){
	return Master2Mem[nodePtr/4 + TREE_LEFT_OFFSET];
}
next_t tree_node_read_right(data_t *Master2Mem, ptr_t nodePtr){
	return Master2Mem[nodePtr/4 + TREE_RIGHT_OFFSET];
}
data_t tree_node_read_data(data_t *Master2Mem, ptr_t nodePtr){
	return Master2Mem[nodePtr/4 + TREE_DATA_OFFSET];
}
