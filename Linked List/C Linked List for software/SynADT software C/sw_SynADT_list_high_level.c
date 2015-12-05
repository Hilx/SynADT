#include "sw_SynADT.h"

int *Add2List(int data, int *hdPtr){	
	int *newNode;
	newNode = (int*)malloc(WORDS_PER_NODE*sizeof(int));

	newNode[0] = data;
	
	//make the new node point at the current head of list
	if(hdPtr == NULL){
		newNode[1] = NULL_PTR -(int)newNode; //hdPtr with value NULL - newNode
	}else{
		newNode[1] = hdPtr - newNode;
	}
	
	hdPtr = newNode; //make the hdList point at the new node	
	
	return hdPtr;
}


/* Reverse the list */
int *ReverseList(int *hdPtr){
	int *currentPtr, *hdOldList, *hdNewList;
	int PtrCalcValue;
	int Flag_Stop;
	
	hdNewList = NULL;
	Flag_Stop = 0;

	while(Flag_Stop == 0){
		currentPtr = hdPtr; //update currentPtr

		//save where the current node is pointing at
		PtrCalcValue = (int)currentPtr + currentPtr[1];
		if(PtrCalcValue != 0){
			hdOldList = currentPtr + currentPtr[1];
		}else{
			//we are processing the last node in the old list!
			Flag_Stop = 1;	
		}
		
		// update where this node is pointing at by updating the offset stored
		if(hdNewList == NULL){
			currentPtr[1] = NULL_PTR - (int)currentPtr;
			
		}else{
			currentPtr[1] = hdNewList - currentPtr;		
		}
		
		hdNewList = currentPtr; //update the pointer destination for next iteration
		
		if(PtrCalcValue != 0){
			hdPtr = hdOldList;//make hdList point at the rest of the original list
		}
	}
	hdPtr = currentPtr; //make hdList point at the head of the reversed list
	
	return hdPtr;	
}

/* Delete the list */
int *DeleteList(int *hdPtr){
	int *nowPtr;
	int nextPtr_value;
	
	while(hdPtr != NULL){
		nowPtr = hdPtr;
		nextPtr_value = (int)nowPtr + nowPtr[1];
		if(nextPtr_value != 0){
			hdPtr = nowPtr + nowPtr[1];
		}else{
			hdPtr = NULL;
		}
		free(nowPtr);
	}
	return hdPtr;
}