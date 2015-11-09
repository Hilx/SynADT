#include <stdio.h>
#include <stdlib.h>

#define WORDS_PER_NODE 2
#define NULL_PTR 0

int *ListGen(int SizeOfList, int *hdPtr); 
int *Add2List(int data, int *hdPtr);
int *ReverseList(int *hdPtr);
int *DeleteList(int *hdPtr);
int *DeleteListNode(int *hdPtr, int key);
void PrintList(int *hdPtr);


int main(){
	
	int *hdPtr = NULL;
	int SizeOfList = 10;

	
	hdPtr = ListGen(10,hdPtr);
	PrintList(hdPtr);

	//hdPtr = ReverseList(hdPtr);
	//PrintList(hdPtr);
	
	//hdPtr = DeleteList(hdPtr);
	//PrintList(hdPtr);
	
	hdPtr = DeleteListNode(hdPtr, 5);
	PrintList(hdPtr);
	
	hdPtr = DeleteListNode(hdPtr, 0);
	PrintList(hdPtr);	
	
	hdPtr = DeleteListNode(hdPtr, 9);
	PrintList(hdPtr);		

	return 0;
}

/* Creating a list */
int *ListGen(int SizeOfList, int *hdPtr){
	int i;
	int input[10] = {5,3,9,4,1,2,8,7,0,6};
	for(i = 0; i< SizeOfList;i++){
		hdPtr = Add2List(input[i], hdPtr);
	}
	
	return hdPtr;
}

int *Add2List(int data, int *hdPtr){	
	/*
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
	*/
	int *insertPtr;
	int *currentPtr, *nextPtr;
	int currentKey, nextKey;
	int nextPtr_value;
	
	// search for insertion point
	if(hdPtr == NULL){
		insertPtr = hdPtr;
	}else{
		// search for appropriate point to insert the new node
		currentPtr = hdPtr;
		nextPtr_value = (int)currentPtr + currentPtr[1];
		if(nextPtr_value != 0){
			nextPtr = currentPtr + currentPtr[1];
			nextKey = nextPtr[0];
		}else{
			nextPtr = NULL;
		}
		currentKey = currentPtr[0];
		// while not the end of list && data
		if(data > currentKey){
			while(nextPtr != NULL && data > nextKey){
				currentPtr = nextPtr;
				
				nextPtr_value = (int)currentPtr + currentPtr[1];
				if(nextPtr_value != 0){
					nextPtr = currentPtr + currentPtr[1];
					nextKey = nextPtr[0];
				}else{
					nextPtr = NULL;
				}	
			}
			insertPtr = currentPtr;			
		}
	}
	// insert
	int *newNode;
	newNode = (int*)malloc(WORDS_PER_NODE*sizeof(int));
	newNode[0] = data;
	if(data > currentKey){
		if(insertPtr == NULL){
			newNode[1] = NULL_PTR -(int)newNode;
			hdPtr = newNode;
		}else{
			insertPtr[1] = newNode - insertPtr;
			if(nextPtr != NULL){
				newNode[1] = nextPtr - newNode;
			}else{
				newNode[1] = NULL_PTR - (int)newNode;
			}
			hdPtr = hdPtr;
		}	
	}else{
		newNode[1] = hdPtr - newNode;
		hdPtr = newNode;
	}
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

int *DeleteListNode(int *hdPtr, int key){
	
	int *currentPtr = hdPtr;
	int currentKey = currentPtr[0];
	int nextPtr_value;
	int *nextPtr, *savePtr;
	int flag_search = 0;
	// search
	while(currentKey != key && currentPtr != NULL){
		flag_search = 1;
		
		nextPtr_value = (int)currentPtr + currentPtr[1];
		if(nextPtr_value != 0){
			nextPtr = currentPtr + currentPtr[1];
		}else{
			nextPtr = NULL;
		}			
		savePtr = currentPtr;
		currentPtr = nextPtr;
		currentKey = currentPtr[0];
	}
	
	// delete	
	if(flag_search == 0){
		nextPtr_value = (int)currentPtr + currentPtr[1];
		if(nextPtr_value != 0){
			nextPtr = currentPtr + currentPtr[1];
		}else{
			nextPtr = NULL;
		}		
		hdPtr = nextPtr;
		free(currentPtr);		
	}else{
		nextPtr_value = (int)currentPtr + currentPtr[1];
		if(nextPtr_value != 0){
			nextPtr = currentPtr + currentPtr[1];
			savePtr[1] = nextPtr - savePtr;
		}else{
			savePtr[1] = NULL_PTR - (int)savePtr;
			nextPtr = NULL;
		}
		free(currentPtr);
		hdPtr = hdPtr;
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