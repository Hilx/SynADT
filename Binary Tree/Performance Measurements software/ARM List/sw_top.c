#include <stdio.h>
#include "platform.h"
#include "sw_top.h"


int main()
{
	init_platform();
	//init_ddr(); // uncomment if using SysAlloc
	print("Hello World\n\r");

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

	int log2_tree_size;

	int *root;
	int i,j;
	for(i = 0; i < 21; i++){

		log2_tree_size = i;

		root = NULL;

		xor_rng_init();
		//init_ddr();

		putnum(i);
		print(" ");

		Xil_Out32(COUNTER_BASE + 4 * 1, START);
		root = PM_1_INSERTION(root, log2_tree_size);
		Xil_Out32(COUNTER_BASE + 4 * 1, STOP);

		Xil_Out32(COUNTER_BASE + 4 * 2, START);
		root = PM_2_CHECK_INSERTION(root, log2_tree_size);
		Xil_Out32(COUNTER_BASE + 4 * 2, STOP);

		Xil_Out32(COUNTER_BASE + 4 * 3, START);
		root = PM_3_UPDATE(root, log2_tree_size);
		Xil_Out32(COUNTER_BASE + 4 * 3, STOP);

		Xil_Out32(COUNTER_BASE + 4 * 4, START);
		root = PM_4_DELETION(root);
		Xil_Out32(COUNTER_BASE + 4 * 4, STOP);

		// read counter result back
		for(j = 1; j<5; j++){
			if(i > 17){
				putnum(Xil_In32(COUNTER_BASE + 4 * (j + 8) ));
			}
			putnum(Xil_In32(COUNTER_BASE + 4 * j));
			print(" ");
			// reset counter
			Xil_Out32(COUNTER_BASE + 4 * j, RESET);
		}
		print("\n");
	}
	print("\n all done\n");

	cleanup_platform();
	return 0;
}