#include "sw_top.h"

/* Creating a list */
int *ListGen(int SizeOfList, int *hdPtr){
	int i;
	for(i = 0; i< SizeOfList;i++){		
		hdPtr = Add2List(i, hdPtr);
	}
	return hdPtr;
}

/* Performance Measurements List Generation*/
int *RandListGen(int log2SizeOfList, int *hdPtr){
	int i;
	int iter = 1 << log2SizeOfList;
	for(i = 0; i< iter;i++){		
		hdPtr = Add2List(RandGen(log2SizeOfList), hdPtr);
	}
	return hdPtr;
}

/* SOFTWARE ONLY */
/* Print the list*/
void PrintList(int *hdPtr){	
	if(hdPtr != NULL){
		int *nowPtr = hdPtr;
		printf("Printing List [");
		while(nowPtr != NULL){
			printf(" %d",list_node_read_data(nowPtr));		
			nowPtr = list_get_next_pointer(nowPtr);
		}	
		printf("]\n");	
	}else{
		printf("The list is empty.");
	}
}