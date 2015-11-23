#include <stdio.h>
#include "platform.h"

void print(char *str);

/* SysAlloc */
#define MMU_BASE 0x43C00000 // change this to your SysAlloc base address
#define MMU_TOKEN 0
#define MMU_STATUS 4
#define MMU_RESULT 8
#define MMU_CMD 12
#define MMU_FREE 16

int hw_malloc(int size);
void hw_mfree(int address);
void init_ddr(void);


/* Counter */
#define COUNTER_BASE 0x43C20000 // change according to system
#define START 1
#define STOP 2
#define RESET 3

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

int main()
{
	init_platform();
	init_ddr();
	print("Hello World\n\r");
	/*
	int k;

	NodePtr newNode;
	for(k = 0; k < 505; k++){
		putnum(k);
		print(" ");
		newNode = (struct Node *)malloc(sizeof(struct Node));
		putnum(newNode);
		print("\n");
	}
	*/
	/*
	int *helloPtr;
	helloPtr = 0x00000000;
	int temp;

	putnum(helloPtr);
	print(" ");
	putnum(*helloPtr);

*/


	//temp = *helloPtr + 5;
	// putnum(temp);




	NodePtr hdList = NULL; // a pointer which points at the head node of the list

	int ListSize = 0;

	// Creating a list
	Xil_Out32(COUNTER_BASE + 4 * 1, START);
	ListGen(ListSize, &hdList);
	Xil_Out32(COUNTER_BASE + 4 * 1, STOP);
	//PrintList(&hdList);

	// Processing the list - Reverse it
	Xil_Out32(COUNTER_BASE + 4 * 2, START);
	ReverseList(&hdList);
	Xil_Out32(COUNTER_BASE + 4 * 2, STOP);
	//PrintList(&hdList);

	// Delete the list
	Xil_Out32(COUNTER_BASE + 4 * 3, START);
	DeleteList(&hdList);
	Xil_Out32(COUNTER_BASE + 4 * 3, STOP);
	//PrintList(&hdList);

	print("Creating ");
	putnum(Xil_In32(COUNTER_BASE + 4 * 1));
	print("\n");

	print("Reversing ");
	putnum(Xil_In32(COUNTER_BASE + 4 * 2));
	print("\n");

	print("Destroying ");
	putnum(Xil_In32(COUNTER_BASE + 4 * 3));
	print("\n");

	cleanup_platform();
	return 0;
}

/* Creating a li
* st */
void ListGen(int ListSize, NodePtr *hdList){
	int i;
	for(i = 0; i < ListSize; i++){
		Add2List(i, hdList);

	}
}
void Add2List(int data, NodePtr *hdList){
	NodePtr newNode;
	//newNode = (struct Node *)malloc(sizeof(struct Node));
	newNode = hw_malloc(sizeof(struct Node));
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
		//free(nowPtr);
		hw_mfree(nowPtr);
	}
}

/* SOFTWARE ONLY */
/* Print the list*/
void PrintList(NodePtr *hdList){
	NodePtr nowPtr;
	nowPtr = *hdList;

	if (nowPtr != NULL){
		print("Printing List...\n");
		while(nowPtr != NULL){
			//printf("%d ",nowPtr->data);
			putnum(nowPtr->data);
			print(" ");

			nowPtr = nowPtr->next;
			print("\n");
		}
		print(" <- That's all!\n");
	}else{
		print("The list is empty.");
	}
}

int hw_malloc(int size){
	int token,status,result;
	token = 0;
	status = 0;

	// acquiring token
	while(token == 0){
		token =  Xil_In32(MMU_BASE + MMU_TOKEN);
	}

	// write request
	Xil_Out32(MMU_BASE + MMU_CMD, size);

	// checking allocation status
	while(status == 0){
		status =  Xil_In32(MMU_BASE + MMU_STATUS);
	}

	//read result back
	result = Xil_In32(MMU_BASE + MMU_RESULT);

	return result;
}

void hw_mfree(int address){
	//print("Free\n");
	int token,status,result;
	token = 0;
	status = 0;

	// acquiring token
	while(token == 0){
		token =  Xil_In32(MMU_BASE + MMU_TOKEN);
	}

	// write request
	Xil_Out32(MMU_BASE + MMU_FREE, address);

	// checking de-allocation status
	while(status == 0){
		status =  Xil_In32(MMU_BASE + MMU_STATUS);
	}

	// performing a READ to reset token
	result = Xil_In32(MMU_BASE + MMU_RESULT);
	//print("Done Free\n");
}

void init_ddr(void){
	int i;
	for(i = 0; i <2396745; i++)
	Xil_Out32(0x18000000 + 4*i, 0);

}
