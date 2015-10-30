#include "main.h"

struct stack_t myStack(int *hdPtr, int command, ptr_t pointer, int operation){
	struct stack_t output;
	int rightMask = 0x80000000;
	int nodeData;
	int stackOut;
	if(command == WRITE_STACK){
		if(operation == GOING_LEFT){
			nodeData = pointer;
		}else if(operation == GOING_RIGHT){
			nodeData = pointer | rightMask;
		}

	output.hdPtr = Add2List(nodeData, hdPtr);		
		
	}else if(command == READ_STACK){
		//read 
		stackOut = hdPtr[0];
		if((stackOut & 0x80000000) == GOING_LEFT){
			output.operation = GOING_LEFT;
		}else{
			output.operation = GOING_RIGHT;
		}
		output.pointer = (stackOut & 0x7FFFFFFF);
		//delete
		output.hdPtr = DeleteNode(hdPtr);
	}
	return output;
}

/* Creating a list */
int *ListGen(int SizeOfList, int *hdPtr){
	int i;
	for(i = 0; i< SizeOfList;i++){
		hdPtr = Add2List(i, hdPtr);
	}
	
	return hdPtr;
}

int *Add2List(int data, int *hdPtr){	
	int *newNode;
	newNode = (int*)malloc(WORDS_PER_NODE*sizeof(int));

	newNode[0] = data;
	
	//make the new node point at the current head of list
	if(hdPtr == NULL){
		newNode[1] = LIST_NULL_PTR -(int)newNode; //hdPtr with value NULL - newNode
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
			currentPtr[1] = LIST_NULL_PTR - (int)currentPtr;
			
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

int *DeleteNode(int *hdPtr){
	int *nowPtr;
	int nextPtr_value;	
		nowPtr = hdPtr;
		nextPtr_value = (int)nowPtr + nowPtr[1];
		if(nextPtr_value != 0){
			hdPtr = nowPtr + nowPtr[1];
		}else{
			hdPtr = NULL;
		}
		free(nowPtr);
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
			if(nextPtr_value == LIST_NULL_PTR){ // to avoid segementation fault
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