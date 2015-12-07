#include <stdio.h>
#include "platform.h"

#include "xctreeubnew.h"

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
#define TREE_BASE 0x43C10000 // change according to system

/* Debug */
#define DEBUG_BASE 0x15000000

int main()
{
	init_platform();
	init_ddr();
	print("Hello World\n\r");

	int log2size;

	XCtreeubnew myIP;
	int list_set_up;

	int i;
	for(i =0; i < 3; i++){
	//for(i = 20; i<21; i++){//should start from 18
		putnum(i);
		log2size = 6;

		list_set_up = XCtreeubnew_Initialize(&myIP, XPAR_CTREEUBNEW_0_DEVICE_ID);
		list_set_up = XCtreeubnew_IsReady(&myIP);

		Xil_Out32(TREE_BASE + 0x20, log2size);

		// start list IP
		XCtreeubnew_Start(&myIP);

		// check if list IP is done
		int flag_done = 0;
		flag_done = XCtreeubnew_IsDone(&myIP);
		while(flag_done != 1){
			flag_done = XCtreeubnew_IsDone(&myIP);
		}

		print("IP done\n");
		// read counter result back
		if(i > 17){
			putnum(Xil_In32(COUNTER_BASE + 4 * (1 + 8) ));
		}
		putnum(Xil_In32(COUNTER_BASE + 4 * 1));
		print(" ");

		if(i > 17){
			putnum(Xil_In32(COUNTER_BASE + 4 * (2 + 8) ));
		}
		putnum(Xil_In32(COUNTER_BASE + 4 * 2));
		print(" ");

		if(i > 17){
			putnum(Xil_In32(COUNTER_BASE + 4 * (3 + 8) ));
		}
		putnum(Xil_In32(COUNTER_BASE + 4 * 3));
		print(" ");

		if(i > 17){
			putnum(Xil_In32(COUNTER_BASE + 4 * (4 + 8) ));
		}
		putnum(Xil_In32(COUNTER_BASE + 4 * 4));
		print("\n");


		// reset counter
		Xil_Out32(COUNTER_BASE + 4 * 1,RESET);
		Xil_Out32(COUNTER_BASE + 4 * 2,RESET);
		Xil_Out32(COUNTER_BASE + 4 * 3,RESET);
		Xil_Out32(COUNTER_BASE + 4 * 4,RESET);
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
