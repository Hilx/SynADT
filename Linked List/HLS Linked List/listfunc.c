#include "top.h"

/* Memory Node Access - Write Node */
void node_write_next(data_t *Master2Mem, ptr_t nodePtr, next_t offset){
	Master2Mem[nodePtr/4 + NEXT_OFFSET] = offset;
}
void node_write_data(data_t *Master2Mem, ptr_t nodePtr, data_t data){
	Master2Mem[nodePtr/4 + DATA_OFFSET] = data;
}
void node_write(data_t *Master2Mem, ptr_t nodePtr, struct node_t currentNode){
	node_write_data(Master2Mem, nodePtr, currentNode.data);
	node_write_next(Master2Mem, nodePtr, currentNode.next);
}

/* Memory Node Access - Read Node */
next_t node_read_next(data_t *Master2Mem, ptr_t nodePtr){
	return Master2Mem[nodePtr/4 + NEXT_OFFSET];

}
data_t node_read_data(data_t *Master2Mem, ptr_t nodePtr){
	return Master2Mem[nodePtr/4 + DATA_OFFSET];
}
struct node_t node_read(data_t *Master2Mem, ptr_t nodePtr){
	struct node_t readNode;
	readNode.data = node_read_data(Master2Mem, nodePtr);
	readNode.next = node_read_next(Master2Mem, nodePtr);
	return readNode;
}

/* Low-level list manipulation functions */
void node_set_next(data_t *Master2Mem, ptr_t currentNodePtr, ptr_t nextNodePtr){
	next_t offset;
	offset = nextNodePtr - currentNodePtr;
	node_write_next(Master2Mem, currentNodePtr, offset);
}

ptr_t node_alloc_new(data_t *Master2Mem, data_t *Master2SysAlloc, data_t data, ptr_t nextNodePtr){
	ptr_t newNodePtr;
	struct node_t newNode;
	newNodePtr = SysMalloc(sizeof(struct node_t), Master2SysAlloc);
	newNode.data = data;
	newNode.next = nextNodePtr - newNodePtr;
	node_write(Master2Mem, newNodePtr, newNode);
	return newNodePtr;
}

ptr_t node_get_nextNodePtr(data_t *Master2Mem, ptr_t currentNodePtr){
	next_t offset = node_read_next(Master2Mem, currentNodePtr);
	return currentNodePtr + offset;
}

void node_delete(data_t *Master2SysAlloc, ptr_t nodePtr){
	SysFree(nodePtr, Master2SysAlloc);
}

/* Software C level list manipulation functions */
/* Creating a list */
ptr_t ListGen(data_t *Master2Mem, data_t *Master2SysAlloc, int SizeOfList, ptr_t hdPtr){
	int i;
	for(i=0;i<SizeOfList;i++){
		hdPtr = Add2List(Master2Mem, Master2SysAlloc, i, hdPtr);
	}
	return hdPtr;
}
ptr_t Add2List(data_t *Master2Mem, data_t *Master2SysAlloc, data_t data, ptr_t hdPtr){
	hdPtr = node_alloc_new(Master2Mem, Master2SysAlloc, data, hdPtr);
	return hdPtr;
}
/* Reverse the list */
ptr_t ReverseList(data_t *Master2Mem,ptr_t hdPtr){
	ptr_t currentPtr, hdOldListPtr, hdNewListPtr;
	next_t offset;
	
	hdNewListPtr = NULL_PTR;
	while(hdPtr != NULL_PTR){
		currentPtr = hdPtr;
		
		offset = node_read_next(Master2Mem, currentPtr);
		hdOldListPtr = currentPtr + offset;
		
		//offset = hdNewListPtr - currentPtr;
		//node_write_next(Master2Mem, currentPtr, offset);
		node_set_next(Master2Mem, currentPtr, hdNewListPtr);
		
		hdNewListPtr = currentPtr;
		
		hdPtr = hdOldListPtr;		
	}
	hdPtr = currentPtr;	
	
	return hdPtr;
}
/* Delete the list */
ptr_t DeleteList(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdPtr){
	ptr_t nowPtr;
	next_t offset;
	
	while(hdPtr != NULL_PTR){
		nowPtr = hdPtr;		
		hdPtr = node_get_nextNodePtr(Master2Mem, nowPtr);
		node_delete(Master2SysAlloc, nowPtr);
	}
	
	return hdPtr;
}
