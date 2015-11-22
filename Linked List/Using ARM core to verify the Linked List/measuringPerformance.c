#include <stdio.h>
#include "platform.h"

#include "xclist.h"

//void print(char *str);

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

/* List */
#define LIST_BASE 0x43C10000 // change according to system

/* Debug */
#define DEBUG_BASE 0x15000000

int main()
{
	init_platform();
	init_ddr();
	print("Hello World\n\r");


	/*
	int l;
	int addr;
	print("mallocs\n");
	for(l = 0; l < 1000000; l ++){
		addr = hw_malloc(8);
		Xil_Out32(DEBUG_BASE + 4 * l, addr);
	}

	putnum(addr);
	print("frees\n");
	for(l = 0; l < 1000000; l ++){
		addr = Xil_In32(DEBUG_BASE + 4 * l);
		hw_mfree(addr);
	}
	putnum(addr);
	print("done!\n");

	putnum(hw_malloc(8));
*/


	XClist myIP;
	int list_set_up;
	list_set_up = XClist_Initialize(&myIP, XPAR_CLIST_0_DEVICE_ID);
	list_set_up = XClist_IsReady(&myIP);

	Xil_Out32(LIST_BASE + 0x20, 10);

	// start list IP
	XClist_Start(&myIP);


	// check if list IP is done

	int flag_done = 0;
	flag_done = XClist_IsDone(&myIP);
	while(flag_done != 1){
		flag_done = XClist_IsDone(&myIP);
	}


	// read counter result back

	print("Overall ");
	putnum(Xil_In32(COUNTER_BASE + 4 * 0));
	print("\n");

	print("Creating ");
	putnum(Xil_In32(COUNTER_BASE + 4 * 1));
	print("\n");

	print("Reversing ");
	putnum(Xil_In32(COUNTER_BASE + 4 * 2));
	print("\n");

	print("Destroying ");
	putnum(Xil_In32(COUNTER_BASE + 4 * 3));
	print("\n");

	int myPtr;
	myPtr = Xil_In32(LIST_BASE + 0x10);
	putnum(myPtr);
	// putnum(hw_malloc(8));
	/*
	int hdPtr;
	hdPtr = Xil_In32(LIST_BASE + 0x10);

	print("hdPtr = ");
	putnum(hdPtr);
	print("\n");

	signed offset;
	offset = Xil_In32(hdPtr);

	print("offset = ");
	putnum(offset);
	print("\n");

	int data;
	data = Xil_In32(hdPtr + 4);

	print("data = ");
	putnum(data);
	print("\n");
*/
	//-----------
	/*
	hdPtr = hdPtr + offset;

	print("hdPtr = ");
	putnum(hdPtr);
	print("\n");

	offset = Xil_In32(hdPtr);

	print("offset = ");
	putnum(offset);
	print("\n");

	data = Xil_In32(hdPtr + 4);

	print("data = ");
	putnum(data);
	print("\n");
	*/
	//-----------
	/*
	int l;
	int nowPtr;

	l = 0;
	int i;
// while(hdPtr != 0){
	for(i = 0; i <10; i++){
		nowPtr = hdPtr;
		offset = Xil_In32(hdPtr);
		hdPtr = hdPtr + offset;
		hw_mfree(nowPtr);
		putnum(hdPtr);
		print("\n");
	}

	print("finished processing\n");
	putnum(hw_malloc(8));
*/

	cleanup_platform();
	return 0;
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
