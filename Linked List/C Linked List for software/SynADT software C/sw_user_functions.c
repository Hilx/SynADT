#include "sw_top.h"

/* Creating a list */
int *ListGen(int SizeOfList, int *hdPtr){
	int i;
	for(i = 0; i< SizeOfList;i++){
		hdPtr = Add2List(i, hdPtr);
	}
	
	return hdPtr;
}


/* SOFTWARE ONLY */
/* Print the list*/
void PrintList(int *hdPtr){	
	int *nowPtr;
	int nextPtr_value;
	int Flag_Stop;

	nowPtr = hdPtr;
	Flag_Stop = 0;
	if (nowPtr != NULL){ //nowPtr !=0
		printf("Printing List [");
		while(Flag_Stop == 0){ //nowPtr != 0
			printf(" %d",nowPtr[0]);	
			
			//printf("\nnowPtr and its offset = [ %d, %d]\n", nowPtr,nowPtr[1]);					
			nextPtr_value = (int)nowPtr + nowPtr[1];
			//printf("nextPtr_value = [ %d ]", nextPtr_value);
			if(nextPtr_value == NULL_PTR){ // to avoid segementation fault
				Flag_Stop = 1;				
			}else{
				nowPtr = nowPtr + nowPtr[1]; //nextPtr_value
			}
		}	
		printf("]\n");	
	}else{
		printf("The list is empty.");
	}
}