#include "SynADT.h"

ptr_t Add2SortedList(data_t *Master2Mem, data_t *Master2SysAlloc, data_t data, ptr_t hdPtr){
	if(hdPtr == NULL_PTR){
		hdPtr = list_node_alloc_new(Master2Mem, Master2SysAlloc, data, NULL_PTR);
	}else{
		int flag_stop = 0;		
		struct list_node_t_std nowNode;
		ptr_t nowPtr = hdPtr;
		ptr_t prePtr = hdPtr;
SEARCH: while(flag_stop != 1 && nowPtr != NULL_PTR){
			nowNode = list_node_read_std(Master2Mem, nowPtr);
			if(data > nowNode.data){
				flag_stop = 1;	
				ptr_t newNodePtr = list_node_alloc_new(Master2Mem, Master2SysAlloc, data, nowPtr);
				if(prePtr == hdPtr){
					hdPtr = newNodePtr;
				}else{
					list_node_set_next_pointer(Master2Mem, prePtr, newNodePtr);
				}		
			}else if(data < nowNode.data){
				prePtr = nowPtr;
				nowPtr = nowNode.nextPtr;				
				flag_stop = 0;
			}else if(data == nowNode.data){
				flag_stop = 1;
			}
		}
	}
	return hdPtr;
}

ptr_t SearchInList(data_t *Master2Mem, ptr_t hdPtr, int key){
	ptr_t outPtr;
	if(hdPtr == NULL_PTR){
		outPtr = SEARCH_NOT_FOUND;
	}else{		
		int flag_stop = 0;		
		struct list_node_t_std nowNode;
		ptr_t nowPtr = hdPtr;
		while(flag_stop != 1 && nowPtr != NULL_PTR){
			nowNode = list_node_read_std(Master2Mem, nowPtr);			
			if(key == nowNode.data){
				flag_stop = 1;
				outPtr = nowPtr;
			}else if(key > nowNode.data){
				flag_stop = 1;
				outPtr = SEARCH_NOT_FOUND;
			}else if(key < nowNode.data){
				flag_stop = 0;
				nowPtr = nowNode.nextPtr;
			}
		}
	}
	return outPtr;	
}

ptr_t DeleteListNode_HT(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdPtr, int key){
	struct list_node_t_std nowNode;
	ptr_t prePtr, nowPtr;
	if(hdPtr != NULL_PTR){
		int flag_stop = 0;		
		nowPtr = hdPtr;
		while(flag_stop != 1 && nowPtr != NULL_PTR){
			nowNode = list_node_read_std(Master2Mem, nowPtr);			
			if(key < nowNode.data){
				flag_stop = 0;				
				prePtr = nowPtr;
				nowPtr = nowNode.nextPtr;
			}else if(key > nowNode.data){
				flag_stop = 1;
			}else if(key == nowNode.data){
				flag_stop = 1;				
				if(nowPtr == hdPtr){
					hdPtr = nowNode.nextPtr;
					list_node_delete(Master2SysAlloc, nowPtr);
				}else{
					list_node_set_next_pointer(Master2Mem, prePtr, nowNode.nextPtr);
					list_node_delete(Master2SysAlloc, nowPtr);					
				}				
			}
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