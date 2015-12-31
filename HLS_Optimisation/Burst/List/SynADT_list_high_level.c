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
		offset = list_node_read_next(Master2Mem, hdPtr);
		hdPtr = nowPtr + offset;
		list_node_delete(Master2SysAlloc, nowPtr);
	}	
	return hdPtr;
}