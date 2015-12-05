#include "SynADT.h"

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
	
	while(hdPtr != NULL_PTR){
		nowPtr = hdPtr;		
		hdPtr = list_node_get_next_pointer(Master2Mem, nowPtr);
		list_node_delete(Master2SysAlloc, nowPtr);
	}
	
	return hdPtr;
}

ptr_t DeleteListNode(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdPtr){
	ptr_t nowPtr, nextPtr;
		
	nowPtr = hdPtr;
	hdPtr= list_node_get_next_pointer(Master2Mem, nowPtr);
	list_node_delete(Master2SysAlloc, nowPtr);	
	
	return hdPtr;
}
