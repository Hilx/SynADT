#include "SynADT.h"

/* Low-level list manipulation functions */
ptr_t tree_node_alloc_new(data_t *Master2Mem, data_t *Master2SysAlloc, data_t data, ptr_t leftNodePtr, ptr_t rightNodePtr, int *buffPtr){
	ptr_t newNodePtr;
	struct tree_node_t newNode;
	newNodePtr = SysMalloc(REQ_NODE_SIZE, Master2SysAlloc);
	newNode.data = data;
	newNode.left = leftNodePtr - newNodePtr;
	newNode.right = rightNodePtr - newNodePtr;
	tree_node_write(Master2Mem, newNodePtr, newNode, buffPtr);
	return newNodePtr;
}
void tree_node_delete(data_t *Master2SysAlloc, ptr_t nodePtr){
	SysFree(nodePtr, Master2SysAlloc);
}
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

ptr_t tree_node_get_left_pointer(data_t *Master2Mem, ptr_t currentNodePtr){
	next_t offset = tree_node_read_left(Master2Mem, currentNodePtr);
	return currentNodePtr + offset;
}
ptr_t tree_node_get_right_pointer(data_t *Master2Mem, ptr_t currentNodePtr){
	next_t offset = tree_node_read_right(Master2Mem, currentNodePtr);
	return currentNodePtr + offset;
}
/* Burst Both Pointers */
struct pointer_set_t tree_get_both_pointers(data_t *Master2Mem, ptr_t nodePtr, int *buffPtr){
	memcpy(buffPtr, (int*)(Master2Mem + nodePtr/4+ TREE_LEFT_OFFSET), 2*sizeof(int));
	struct pointer_set_t out;
	out.leftPtr = (next_t)buffPtr[TREE_LEFT_OFFSET] + nodePtr;
	out.rightPtr = (next_t)buffPtr[TREE_RIGHT_OFFSET] + nodePtr;
	return out;
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
void tree_node_write(data_t *Master2Mem, ptr_t nodePtr, struct tree_node_t currentNode, int *buffPtr){
	buffPtr[TREE_DATA_OFFSET] = currentNode.data;
	buffPtr[TREE_LEFT_OFFSET] = currentNode.left;
	buffPtr[TREE_RIGHT_OFFSET] = currentNode.right;
	memcpy((int *)(Master2Mem + nodePtr/4), buffPtr, 3*sizeof(int));
}
void tree_node_write_std(data_t *Master2Mem, ptr_t nodePtr, struct tree_node_t_std currentNode, int *buffPtr){
	struct tree_node_t nodeOut;
	nodeOut.data = currentNode.data;
	nodeOut.left = currentNode.leftPtr - nodePtr;
	nodeOut.right = currentNode.rightPtr - nodePtr;
	tree_node_write(Master2Mem, nodePtr, nodeOut, buffPtr);
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
struct tree_node_t tree_node_read(data_t *Master2Mem, ptr_t nodePtr, int *buffPtr){

	memcpy(buffPtr,(int*)(Master2Mem + nodePtr/4), 3*sizeof(int));
	struct tree_node_t readNode;
	readNode.data = buffPtr[TREE_DATA_OFFSET];
	readNode.left = buffPtr[TREE_LEFT_OFFSET];
	readNode.right= buffPtr[TREE_RIGHT_OFFSET];
	return readNode;
}
struct tree_node_t_std tree_node_read_std(data_t *Master2Mem, ptr_t nodePtr, int *buffPtr){
	struct tree_node_t nodeIn = tree_node_read(Master2Mem, nodePtr, buffPtr);
	struct tree_node_t_std nodeOut;
	nodeOut.data = nodeIn.data;
	nodeOut.leftPtr = (next_t)nodeIn.left + nodePtr;
	nodeOut.rightPtr = (next_t)nodeIn.right + nodePtr;
	return nodeOut;
}
