/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* XILINX CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xio.h"

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
void PrintList(NodePtr *hdList);


int main()
{
    init_platform();
    init_ddr();
    print("Hello World\n\r");

    // ------------------
    NodePtr hdList = NULL;
    int ListSize;

   // int i;
   // for(i=2; i > -1; i--){
   // 	print("iteration ");

    	ListSize = 0;

		// create
		XIo_Out32(COUNTER_BASE + 4 * 1, START);
		ListGen(ListSize, &hdList);
		XIo_Out32(COUNTER_BASE + 4 * 1, STOP);
		//PrintList(&hdList);

		// reverse
		XIo_Out32(COUNTER_BASE + 4 * 2, START);
		ReverseList(&hdList);
		XIo_Out32(COUNTER_BASE + 4 * 2, STOP);

		// delete
		XIo_Out32(COUNTER_BASE + 4 * 3, START);
		DeleteList(&hdList);
		XIo_Out32(COUNTER_BASE + 4 * 3, STOP);

		// performance
		putnum(XIo_In32(COUNTER_BASE + 4 * 1));
		print(" ");
		putnum(XIo_In32(COUNTER_BASE + 4 * 2));
		print(" ");
		putnum(XIo_In32(COUNTER_BASE + 4 * 3));

		XIo_Out32(COUNTER_BASE + 4 * 1, RESET);
		XIo_Out32(COUNTER_BASE + 4 * 2, RESET);
		XIo_Out32(COUNTER_BASE + 4 * 3, RESET);
	//	print("\n");
   // }
		print("\ndone\n");
    cleanup_platform();
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
	currentPtr = *hdList;
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


int hw_malloc(int size){
	int token,status,result;
	token = 0;
	status = 0;

	// acquiring token
	while(token == 0){
		token =  XIo_In32(MMU_BASE + MMU_TOKEN);
	}

	// write request
	XIo_Out32(MMU_BASE + MMU_CMD, size);

	// checking allocation status
	while(status == 0){
		status =  XIo_In32(MMU_BASE + MMU_STATUS);
	}

	//read result back
	result = XIo_In32(MMU_BASE + MMU_RESULT);

	return result;
}

void hw_mfree(int address){
	//print("Free\n");
	int token,status,result;
	token = 0;
	status = 0;

	// acquiring token
	while(token == 0){
		token =  XIo_In32(MMU_BASE + MMU_TOKEN);
	}

	// write request
	XIo_Out32(MMU_BASE + MMU_FREE, address);

	// checking de-allocation status
	while(status == 0){
		status =  XIo_In32(MMU_BASE + MMU_STATUS);
	}

	// performing a READ to reset token
	result = XIo_In32(MMU_BASE + MMU_RESULT);
	//print("Done Free\n");
}

void init_ddr(void){
	int i;
	for(i = 0; i <2396745; i++)
	XIo_Out32(0x18000000 + 4*i, 0);

}

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
