#include <stdio.h>
#include "platform.h"
#include "sw_top.h"


int main()
{
	init_platform();
	//init_ddr(); // uncomment if using SysAlloc
	print("Hello World\n\r");

	int log2ListSize;

	/*
	// Code for measuring the latency of RNG
	int k;
	for(k = 0; k <10; k++){
		Xil_Out32(COUNTER_BASE + 4 * k, START);
		RandGen(10);
		Xil_Out32(COUNTER_BASE + 4 * k, STOP);
		putnum(Xil_In32(COUNTER_BASE + 4 * k));
		Xil_Out32(COUNTER_BASE + 4 * k,RESET);
		print("\n");
	}	
	*/

	int *hdList = NULL;
	int i;
	for(i =0; i < 21; i++){

		log2ListSize = i;
		
		hdList = NULL;

		//init_ddr(); // uncomment if using SysAlloc
		xor_rng_init();

		Xil_Out32(COUNTER_BASE + 4 * 1, START);
		hdList = RandListGen(log2ListSize, hdList);
		Xil_Out32(COUNTER_BASE + 4 * 1, STOP);

		Xil_Out32(COUNTER_BASE + 4 * 2, START);
		hdList = ReverseList(hdList);
		Xil_Out32(COUNTER_BASE + 4 * 2, STOP);

		Xil_Out32(COUNTER_BASE + 4 * 3, START);
		hdList = DeleteList(hdList);
		Xil_Out32(COUNTER_BASE + 4 * 3, STOP);

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
	*/
	print("\n all done\n");

	cleanup_platform();
	return 0;
}
