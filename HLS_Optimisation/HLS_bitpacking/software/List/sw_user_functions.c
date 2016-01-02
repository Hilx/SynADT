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
		hdPtr = Add2List(i, hdPtr);
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
			struct node_t_std nowNode = list_node_read_std(nowPtr);
			printf(" %d",nowNode.data);
			nowPtr = nowNode.nextPtr;
		}	
		printf("]\n");	
	}else{
		printf("The list is empty.");
	}
}