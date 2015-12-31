#include "SynADT.h"

ptr_t Add2SortedList(data_t *Master2Mem, data_t *Master2SysAlloc, data_t data, ptr_t hdPtr, int *buffPtr){
	// search insertion point
	int flag_already_there = 0;
	ptr_t insertPtr;
	ptr_t keepPtr;
	if(hdPtr == NULL_PTR){
		insertPtr = NULL_PTR;
	}else{
		int flag_stop = 0;		
		struct list_node_t_std nowNode;
		ptr_t nowPtr = hdPtr;
		while(flag_stop != 1 && nowPtr != NULL_PTR){
			nowNode = list_node_read_std(Master2Mem, nowPtr, buffPtr);
			if(data > nowNode.data){
				insertPtr = nowPtr;
				flag_stop = 1;
			}else if(data < nowNode.data){
				// keep looking
				keepPtr = nowPtr;
				nowPtr = nowNode.nextPtr;
				insertPtr = nowPtr;
				flag_stop = 0;
			}else if(data == nowNode.data){
				flag_already_there = 1;
				flag_stop = 1;
			}
		}
	}	
	// insert node
	ptr_t newNodePtr;
	if(flag_already_there == 0){
		newNodePtr = list_node_alloc_new(Master2Mem, Master2SysAlloc, data, insertPtr, buffPtr);
		if(hdPtr == insertPtr){
			hdPtr = newNodePtr;
		}else{
			list_node_set_next_pointer(Master2Mem, keepPtr, newNodePtr);
		}
	}
	return hdPtr;
}

ptr_t SearchInList(data_t *Master2Mem, ptr_t hdPtr, int key, int *buffPtr){
	ptr_t outPtr;
	if(hdPtr == NULL_PTR){
		outPtr = SEARCH_NOT_FOUND;
	}else{
		outPtr = SEARCH_NOT_FOUND;
		int flag_stop = 0;		
		struct list_node_t_std nowNode;
		ptr_t nowPtr = hdPtr;
		while(flag_stop != 1 && nowPtr != NULL_PTR){
			nowNode = list_node_read_std(Master2Mem, nowPtr, buffPtr);			
			if(key == nowNode.data){
				flag_stop = 1;
				outPtr = nowPtr;
			}else if(key > nowNode.data){
				flag_stop = 1;
				// the key doesn't exist!
			}else if(key < nowNode.data){
				flag_stop = 0;
				nowPtr = nowNode.nextPtr;
			}
		}
	}
	return outPtr;	
}

ptr_t DeleteListNode_HT(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdPtr, int key, int *buffPtr){
    // search
	int flag_no_deletion = 1;
	int deletionNodePtr;
	struct list_node_t_std nowNode;
	ptr_t keepPtr;
	if(hdPtr != NULL_PTR){
		int flag_stop = 0;		
		struct list_node_t_std nowNode;
		ptr_t nowPtr = hdPtr;
		while(flag_stop != 1 && nowPtr != NULL_PTR){
			nowNode = list_node_read_std(Master2Mem, nowPtr, buffPtr);
			if(key == nowNode.data){
				flag_stop = 1;
				flag_no_deletion = 0;
				deletionNodePtr = nowPtr;
			}else if(key > nowNode.data){
				flag_stop = 1;
				// the key doesn't exist!
			}else if(key < nowNode.data){
				flag_stop = 0;
				keepPtr = nowPtr;
				nowPtr = nowNode.nextPtr;
			}
		}
	}	
	// delete
	ptr_t newNodePtr;
	if(flag_no_deletion == 0){
		if(hdPtr == deletionNodePtr){
			hdPtr = nowNode.nextPtr;
			list_node_delete(Master2SysAlloc, deletionNodePtr);
		}else{
			list_node_set_next_pointer(Master2Mem, keepPtr,nowNode.nextPtr);
			list_node_delete(Master2SysAlloc, deletionNodePtr);
		}
	}
	return hdPtr;		
}

void DeleteList(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdPtr){	
	while(hdPtr != NULL_PTR){
		ptr_t nowPtr = hdPtr;		
		hdPtr = list_node_get_next_pointer(Master2Mem, nowPtr);
		list_node_delete(Master2SysAlloc, nowPtr);
	}
}
