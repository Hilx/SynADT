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
		struct node_t_std node = list_node_read_std(currentPtr);
		hdOldList = node.nextPtr;
		node.nextPtr = hdNewList;
		list_node_write_std(currentPtr, node);
		hdNewList = currentPtr; 		
		hdPtr = hdOldList;
	}
	hdPtr = currentPtr;
	return hdPtr;	
}
/* Delete the list */
int *DeleteList(int *hdPtr){
	int *nowPtr;
	while(hdPtr != NULL){
		nowPtr = hdPtr;
		struct node_t_std node = list_node_read_std(hdPtr);
		hdPtr = node.nextPtr;		
		SysFree(nowPtr);
	}
	return hdPtr;
}