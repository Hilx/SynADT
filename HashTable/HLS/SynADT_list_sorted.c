#include "SynADT_header.h"

/* Creating a list */
ptr_t Add2List(data_t *Master2Mem, data_t *Master2SysAlloc, data_t data, ptr_t hdPtr){
	data_t *myHeap = Master2Mem;
	ptr_t insertPtr;
	ptr_t currentPtr, nextPtr;
	int currentKey, nextKey;
	int nextPtr_value;
	
	// search for insertion point
	if(hdPtr == NULL_PTR){
		insertPtr = hdPtr;
	}else{
		// search for appropriate point to insert the new node
		currentPtr = hdPtr;
		nextPtr = list_node_get_next_pointer(myHeap, currentPtr);
		currentKey = list_node_read_data(myHeap, currentPtr);
		nextKey = list_node_read_data(myHeap, nextPtr);
		// while not the end of list && data
		if(data > currentKey){
			while(nextPtr != NULL_PTR && data > nextKey){
				currentPtr = nextPtr;				
				nextPtr = list_node_get_next_pointer(myHeap, currentPtr);
				nextKey = list_node_read_data(myHeap, nextPtr);				
			}
			insertPtr = currentPtr;			
		}
	}

	// insert
	ptr_t newNode;
	if(data > currentKey){
		if(insertPtr == NULL_PTR){
			newNode = list_node_alloc_new(myHeap, Master2SysAlloc, data, NULL_PTR);
			hdPtr = newNode;
		}else{
			newNode = list_node_alloc_new(myHeap, Master2SysAlloc, data, nextPtr);
			list_node_set_next(myHeap, insertPtr, newNode);
			hdPtr = hdPtr;
		}	
	}else{
		newNode = list_node_alloc_new(myHeap, Master2SysAlloc, data, hdPtr);		
		hdPtr = newNode;
	}
	return hdPtr;
}
ptr_t DeleteListNode_HT(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdPtr, int key){
	data_t *myHeap = Master2Mem;
	ptr_t currentPtr = hdPtr;
	int currentKey = list_node_read_data(myHeap, currentPtr);
	ptr_t nextPtr, savePtr;
	int flag_search = 0;
	
	// search
	while(currentKey != key && currentPtr != NULL_PTR){
		flag_search = 1;
		nextPtr = list_node_get_next_pointer(myHeap, currentPtr);
		
		savePtr = currentPtr;
		currentPtr = nextPtr;
		currentKey = list_node_read_data(myHeap, currentPtr);
	}
	
	// delete	
	if(flag_search == 0){
		nextPtr = list_node_get_next_pointer(myHeap, currentPtr);
		hdPtr = nextPtr;
		list_node_delete(Master2SysAlloc, currentPtr);		
	}else{
		nextPtr = list_node_get_next_pointer(myHeap, currentPtr);	
		list_node_set_next(myHeap, savePtr, nextPtr);
		list_node_delete(Master2SysAlloc, currentPtr);
		hdPtr = hdPtr;
	}
	
	return hdPtr;		
}
ptr_t SearchInList(data_t *Master2Mem, ptr_t hdPtr, int key){
	data_t *myHeap = Master2Mem;
	ptr_t currentPtr = hdPtr;
	int currentKey = list_node_read_data(myHeap, currentPtr);
	int nextPtr;
	while(currentKey != key && currentPtr != NULL_PTR){
		nextPtr = list_node_get_next_pointer(myHeap, currentPtr);
		currentPtr = nextPtr;
		currentKey = list_node_read_data(myHeap, currentPtr);
	}
	
	int outputPtr;
	if(currentPtr == NULL_PTR){
		outputPtr = SEARCH_NOT_FOUND;
	}else{
		outputPtr = currentPtr;
	}
	return outputPtr;	
}