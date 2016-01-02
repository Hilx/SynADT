#include "SynADT.h"

ptr_t Add2List(data_t *Master2Mem, data_t *Master2SysAlloc, data_t data, ptr_t hdPtr, int *buffPtr){
	hdPtr = list_node_alloc_new(Master2Mem, Master2SysAlloc, data, hdPtr, buffPtr);
	return hdPtr;
}
/* Reverse the list */
ptr_t ReverseList(data_t *Master2Mem,ptr_t hdPtr){
	ptr_t currentPtr,hdOldListPtr, hdNewListPtr;
	next_t offset;	
	currentPtr = hdPtr;
	hdNewListPtr = NULL_PTR;
	while(hdPtr != NULL_PTR){
		currentPtr = hdPtr;					
		struct list_node_t_std node = list_node_read_std(Master2Mem, currentPtr, NULL);
		hdOldListPtr = node.nextPtr;
		node.nextPtr = hdNewListPtr;
		list_node_write_std(Master2Mem, currentPtr, node, NULL);
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
		struct list_node_t_std node = list_node_read_std(Master2Mem, hdPtr, NULL);
		hdPtr = node.nextPtr;		
		list_node_delete(Master2SysAlloc, nowPtr);
	}	
	return hdPtr;
}
