#include <stdio.h>
#include "platform.h"
#include "sw_top.h"


int main()
{
	init_platform();
	print("Hello World\n\r");


	TestCase = USE_RANDOM_KEYS;

	int i,j;
	for(i = 20; i < 21; i++){
		SysAlloc_init();
		xor_rng_init();

		int log2_tree_size = i;
		int *root = NULL;

		putnum(log2_tree_size);
		print(" ");

		// read counter result back
		for(j = 1; j<5; j++){
			if(j == 1){
				Xil_Out32(COUNTER_BASE + 4 * 1, START);
				root = PM_1_INSERTION(root, log2_tree_size);
				Xil_Out32(COUNTER_BASE + 4 * 1, STOP);

			}
			if(j == 2){
				Xil_Out32(COUNTER_BASE + 4 * 2, START);
				root = PM_2_CHECK_INSERTION(root, log2_tree_size);
				Xil_Out32(COUNTER_BASE + 4 * 2, STOP);

			}
			if(j == 3){
				Xil_Out32(COUNTER_BASE + 4 * 3, START);
				root = PM_3_UPDATE(root, log2_tree_size);
				Xil_Out32(COUNTER_BASE + 4 * 3, STOP);
			}
			if(j == 4){
				Xil_Out32(COUNTER_BASE + 4 * 4, START);
				root = PM_4_DELETION(root);
				Xil_Out32(COUNTER_BASE + 4 * 4, STOP);
			}

			int read_value = Xil_In32(COUNTER_BASE + 4 * (j + 8));
			if(read_value != 0){
				putnum(read_value);
			}
			putnum(Xil_In32(COUNTER_BASE + 4 * j));
			print(" ");
			Xil_Out32(COUNTER_BASE + 4 * j, RESET);
		}
		print("\n");
	}

	print("\n all done\n");

	cleanup_platform();
	return 0;
}
