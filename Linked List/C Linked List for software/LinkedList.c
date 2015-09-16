//	Created by Hilda Xue on 16 Sept 2015 									 
//	This program 1. creates a list 2. reverses the list 3. deletes the list        
									 
#include <stdio.h>
#include <stdlib.h>

/* Linked List Node */
struct Node {
	int data;
	struct Node *next;	
};

typedef struct Node *NodePtr; // a pointer pointing at a node

/* Function Prototypes */
// Creating a list 
void ListGen(int ListSize, NodePtr *hdList); 
void Add2List(int data, NodePtr *hdList);
 //Processing (Reverse) the list 
void ReverseList(NodePtr *hdList);
// Delete the list 
void DeleteList(NodePtr *hdList);
/* SOFTWARE ONLY function*/
void PrintList(NodePtr *hdList);

int main(){
	
	NodePtr hdList; // a pointer which points at the head node of the list
	
	int ListSize = 25;

 	/* Creating a list */
	ListGen(ListSize, &hdList);
	PrintList(&hdList);
	
	/* Processing the list - Reverse it */
	ReverseList(&hdList);
	PrintList(&hdList);
	
	/* Delete the list */
	DeleteList(&hdList);
	PrintList(&hdList); 

	return 0;
	
}

/* Creating a list */
void ListGen(int ListSize, NodePtr *hdList){
	int i;
	for(i = 0; i < ListSize; i++){
		Add2List(i, hdList);
	}	
}
void Add2List(int data, NodePtr *hdList){	
	NodePtr newNode;
	newNode = (struct Node *)malloc(sizeof(struct Node));
	newNode->data = data;	
	newNode->next = *hdList; //make the new node point at the current head of list
	*hdList = newNode; //make the hdList point at the new node
}

/* Reverse the list */
void ReverseList(NodePtr *hdList){
	NodePtr currentPtr; 
	NodePtr saverPtr;
	NodePtr hdList_temp;
	
	hdList_temp = NULL;
	
	while(*hdList != NULL){	

		currentPtr = *hdList; //update current pointer
		saverPtr = currentPtr->next; //save information about where this node is pointing at
		currentPtr->next = hdList_temp; //update where this node is pointing at
		hdList_temp = currentPtr; //update the pointer destination for next iteration
		*hdList = saverPtr;//make hdList point at the rest of the original list
	}
	
	*hdList = currentPtr; //make hdList point at the head of the reversed list
}

/* Delete the list */
void DeleteList(NodePtr *hdList){
	NodePtr nowPtr;
	
	while(*hdList != NULL){		
		nowPtr = *hdList;
		*hdList = nowPtr->next;
		free(nowPtr);
	}	
}

/* SOFTWARE ONLY */
/* Print the list*/
void PrintList(NodePtr *hdList){
	NodePtr nowPtr;
	nowPtr = *hdList;
	
	if (nowPtr != NULL){
		printf("Printing List...\n");
		while(nowPtr != NULL){
			printf("%d ",nowPtr->data);
			nowPtr = nowPtr->next;		
		}	
		printf(" <- That's all!\n");	
	}else{
		printf("The list is empty.");
	}
}