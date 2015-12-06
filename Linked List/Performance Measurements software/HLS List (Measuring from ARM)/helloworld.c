#include <stdio.h>
#include "platform.h"

#include "xClistnew.h"

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

	int log2ListSize;

	XClistnew myIP;
	int list_set_up;

	int i;
	for(i = 0; i < 21; i++){

		log2ListSize = i;

		list_set_up = XClistnew_Initialize(&myIP, XPAR_CLISTNEW_0_DEVICE_ID);
		list_set_up = XClistnew_IsReady(&myIP);

		Xil_Out32(LIST_BASE + 0x20, log2ListSize);

		// start list IP
		XClistnew_Start(&myIP);

		// check if list IP is done
		int flag_done = 0;
		flag_done = XClistnew_IsDone(&myIP);
		while(flag_done != 1){
			flag_done = XClistnew_IsDone(&myIP);
		}

		// read counter result back
		putnum(Xil_In32(COUNTER_BASE + 4 * 1));
		print(" ");
		putnum(Xil_In32(COUNTER_BASE + 4 * 2));
		print(" ");
		putnum(Xil_In32(COUNTER_BASE + 4 * 3));
		print("\n");

		// reset counter
		Xil_Out32(COUNTER_BASE + 4 * 1,RESET);
		Xil_Out32(COUNTER_BASE + 4 * 2,RESET);
		Xil_Out32(COUNTER_BASE + 4 * 3,RESET);
	}
	print("\n all done\n");

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
