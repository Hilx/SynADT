#include <stdio.h>
#include "platform.h"
#include "sw_top.h"
#include "xio.h"

int main()
{
	init_platform();
	//init_ddr(); // uncomment if using SysAlloc
	print("Hello World\n\r");

	/*
	// code for measuring latency of RNG
	int k;
	for(k =0; k<10; k++){
		XIo_Out32(COUNTER_BASE + 4 * k, START);
		RandGen(10);
		XIo_Out32(COUNTER_BASE + 4 * k, STOP);
		putnum(XIo_In32(COUNTER_BASE + 4 * k));
		XIo_Out32(COUNTER_BASE + 4 * k,RESET);
		print("\n");
	}
	*/

	int *hdList = NULL;
	int i;
	for(i = 0; i < 21; i++){

		log2ListSize = i;
		
		hdList = NULL;

		//init_ddr(); // uncomment if using SysAlloc
		xor_rng_init();

		XIo_Out32(COUNTER_BASE + 4 * 1, START);
		hdList = RandListGen(log2ListSize, hdList);
		XIo_Out32(COUNTER_BASE + 4 * 1, STOP);
		
		XIo_Out32(COUNTER_BASE + 4 * 2, START);
		hdList = ReverseList(hdList);
		XIo_Out32(COUNTER_BASE + 4 * 2, STOP);

		XIo_Out32(COUNTER_BASE + 4 * 3, START);
		hdList = DeleteList(hdList);
		XIo_Out32(COUNTER_BASE + 4 * 3, STOP);

		// read counter result back
		putnum(XIo_In32(COUNTER_BASE + 4 * 1));
		print(" ");
		putnum(XIo_In32(COUNTER_BASE + 4 * 2));
		print(" ");
		putnum(XIo_In32(COUNTER_BASE + 4 * 3));
		print("\n");

		// reset counter
		XIo_Out32(COUNTER_BASE + 4 * 1,RESET);
		XIo_Out32(COUNTER_BASE + 4 * 2,RESET);
		XIo_Out32(COUNTER_BASE + 4 * 3,RESET);
	}
	
	print("\n all done\n");

	cleanup_platform();
	return 0;
}
