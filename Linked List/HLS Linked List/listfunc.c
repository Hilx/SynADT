#include "top.h"

/* Memory Node Access - Write Node */
void list_node_write_next(data_t *Master2Mem, ptr_t nodePtr, next_t offset){
	Master2Mem[nodePtr/4 + LIST_NEXT_OFFSET] = offset;
}
void list_node_write_data(data_t *Master2Mem, ptr_t nodePtr, data_t data){
	Master2Mem[nodePtr/4 + LIST_DATA_OFFSET] = data;
}
void list_node_write(data_t *Master2Mem, ptr_t nodePtr, struct list_node_t currentNode){
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

ptr_t list_node_get_nextNodePtr(data_t *Master2Mem, ptr_t currentNodePtr){
	next_t offset = list_node_read_next(Master2Mem, currentNodePtr);
	return currentNodePtr + offset;
}

void list_node_delete(data_t *Master2SysAlloc, ptr_t nodePtr){
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
	hdPtr = list_node_alloc_new(Master2Mem, Master2SysAlloc, data, hdPtr);
	return hdPtr;
}
/* Reverse the list */
ptr_t ReverseList(data_t *Master2Mem,ptr_t hdPtr){
	ptr_t currentPtr, hdOldListPtr, hdNewListPtr;
	next_t offset;
	
	hdNewListPtr = NULL_PTR;
	while(hdPtr != NULL_PTR){
		currentPtr = hdPtr;
		
		offset = list_node_read_next(Master2Mem, currentPtr);
		hdOldListPtr = currentPtr + offset;

		list_node_set_next(Master2Mem, currentPtr, hdNewListPtr);
		
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
		hdPtr = list_node_get_nextNodePtr(Master2Mem, nowPtr);
		list_node_delete(Master2SysAlloc, nowPtr);
	}
	
	return hdPtr;
}
