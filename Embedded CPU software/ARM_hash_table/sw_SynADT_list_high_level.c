#include "sw_SynADT.h"


/* SOFTWARE ONLY */
/* Print the list*/
void PrintList(int *hdPtr){	
	if(hdPtr != NULL){
		int *nowPtr = hdPtr;
		printf("-> [");
		while(nowPtr != NULL){
			printf(" %d",list_node_read_data(nowPtr));		
			nowPtr = list_get_next_pointer(nowPtr);

		}	
		printf("]\n");	
	}else{
		printf(" empty.\n");
	}
}

/* Add node to list */
int *Add2SortedList(int key, int *hdPtr){	 
	if(hdPtr == NULL){
		// end of list, create node
		hdPtr = list_node_alloc_new(key, hdPtr);
	}else{
		// search for place to insert
		struct node_t_std nowNode = list_node_read_std(hdPtr);
		if(key > nowNode.data){
			hdPtr = list_node_alloc_new(key, hdPtr);
		}else if(key < nowNode.data){
			int *tempPtr = Add2SortedList(key, nowNode.nextPtr);
			list_set_next_pointer(hdPtr, tempPtr);
		}else if(key == nowNode.data){
			//do not insert
			hdPtr = hdPtr;
		}		
	}
	return hdPtr;
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
		//printf("nowPtr = %d\n", nowPtr);
		hdPtr = list_get_next_pointer(nowPtr);		
		SysFree(nowPtr);
	}
	return hdPtr;
}

/* ------- Hashtable Use ---------- */
int *SearchInList(int *hdList, int key){
	if(hdList == NULL){
		return NULL;
	}else{
		int *currentPtr = hdList;
		struct node_t_std nowNode = list_node_read_std(currentPtr);
		while(key < nowNode.data && nowNode.nextPtr != NULL){
			currentPtr = nowNode.nextPtr;
			nowNode = list_node_read_std(currentPtr);		
		}
		
		if(key == nowNode.data){
			return currentPtr;
		}else{
			return NULL;
		}
	}
}
int *DeleteListNode_HT(int *hdPtr, int key){
	if(hdPtr == NULL){
		return NULL;
	}else{
		
		int flag_search = 0;	
		int *currentPtr = hdPtr;
		int *savePtr;
		struct node_t_std nowNode = list_node_read_std(currentPtr);
		while(key < nowNode.data && nowNode.nextPtr != NULL){
			flag_search = 1;
			savePtr = currentPtr;
			currentPtr = nowNode.nextPtr;
			nowNode = list_node_read_std(currentPtr);		
		}
		
		if(key == nowNode.data){	
			if(flag_search == 1){
				list_set_next_pointer(savePtr, nowNode.nextPtr);
				list_node_delete(currentPtr);			
			}else{
				// deleting the head node
				hdPtr = nowNode.nextPtr;
				list_node_delete(currentPtr);
			}
		}
	}
	return hdPtr;	
}
