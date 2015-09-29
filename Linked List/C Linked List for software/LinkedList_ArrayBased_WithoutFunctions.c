//	Created by Hilda Xue on 29 Sept 2015 			
//  This code implements a linked list WITHOUT ADT(Abstract Data Type) 
//  (ADT example: struct Node {int data; Node *next;})
//	Instead, this code uses integer arrays to implement a linked list.						 
//	This program 1. creates a list 2. reverses the list 3. deletes the list     

#include <stdio.h>
#include <stdlib.h>

#define WORDS_PER_NODE 2
#define NULL_PTR 0

void PrintList(int *hdPtr);

int main(){
	
	int *hdPtr;
	int SizeOfList = 10;
	int data = 1;
	int i;
	
	/* Creating a list */
	hdPtr = NULL;
	int *newNode;
	for(i = 0; i <SizeOfList; i++){
		data = i;
		newNode = (int*)malloc(WORDS_PER_NODE*sizeof(int));

		newNode[0] = data;
		
		//make the new node point at the current head of list
		if(hdPtr == NULL){
			newNode[1] = NULL_PTR -(int)newNode; //hdPtr with value NULL - newNode
		}else{
			newNode[1] = hdPtr - newNode;
		}
		
		hdPtr = newNode; //make the hdList point at the new node		
	}

	PrintList(hdPtr);
	
	/* Processing the list - Reverse it */
	
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
	
	PrintList(hdPtr);
	
	/* Delete the list */
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

	PrintList(hdPtr);
	

	return 0;
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