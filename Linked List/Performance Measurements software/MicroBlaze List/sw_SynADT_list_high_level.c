#include "sw_SynADT.h"

/* Add node to list */
int *Add2List(int data, int *hdPtr){	  
	return list_node_alloc_new(data, hdPtr);
}
/* Reverse the list */
int *ReverseList(int *hdPtr){
	int *currentPtr = hdPtr;
	int *hdOldList = hdPtr;
	int *hdNewList = NULL;	
	while(hdOldList != NULL){
		currentPtr = hdPtr;		
		hdOldList = list_get_next_pointer(currentPtr);		
		list_set_next_pointer(currentPtr, hdNewList);		
		hdNewList = currentPtr; 		
		hdPtr = hdOldList;
	}
	hdPtr = currentPtr; //make hdList point at the head of the reversed list	
	return hdPtr;	
}

/* Delete the list */
int *DeleteList(int *hdPtr){
	int *nowPtr;
	while(hdPtr != NULL){
		nowPtr = hdPtr;
		hdPtr = list_get_next_pointer(nowPtr);		
		SysFree(nowPtr);
	}
	return hdPtr;
}