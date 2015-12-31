#include "SynADT.h"


ptr_t Add2List(data_t *Master2Mem, data_t *Master2SysAlloc, data_t data, ptr_t hdPtr, int *buffPtr){
	hdPtr = list_node_alloc_new(Master2Mem, Master2SysAlloc, data, hdPtr, buffPtr);
	return hdPtr;
}

ptr_t DeleteListNode(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdPtr){
	ptr_t nowPtr, nextPtr;
		
	nowPtr = hdPtr;
	hdPtr = list_node_get_next_pointer(Master2Mem, nowPtr);
	list_node_delete(Master2SysAlloc, nowPtr);	
	
	return hdPtr;
}
