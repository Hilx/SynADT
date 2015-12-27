#include <stdio.h>
#include "platform.h"
#include "sw_top.h"
#include "xio.h"

int main(){
	init_platform();
	printf("hello world \n");

	int *hdVector;
	int i,j;
	for(i = 0; i < 21; i++){

		SysAlloc_init();

		xor_rng_init();

		int log2size = i;

		int *hdVector;

		putnum(log2size);
		print(" ");


		// read counter result back
		for(j = 1; j <4 ; j++){
			if(j == 1){
				XIo_Out32(COUNTER_BASE + 4 * 1, START);
				hdVector = PM1_INSERT(log2size);
				XIo_Out32(COUNTER_BASE + 4 * 1, STOP);
			}
			if(j == 2){
				XIo_Out32(COUNTER_BASE + 4 * 2, START);
				PM2_SEQ_ACCESS(hdVector, log2size);
				XIo_Out32(COUNTER_BASE + 4 * 2, STOP);
			}
			if(j == 3){
				XIo_Out32(COUNTER_BASE + 4 * 3, START);
				PM3_RANDOM_ACCESS(hdVector, log2size);
				XIo_Out32(COUNTER_BASE + 4 * 3, STOP);

			}


			// latency
			int read_value = XIo_In32(COUNTER_BASE + 4 * (j + 8));
			if(read_value != 0){
				putnum(read_value);
			}
			putnum(XIo_In32(COUNTER_BASE + 4 * j));
			print(" ");
			// reset counter
			XIo_Out32(COUNTER_BASE + 4 * j, RESET);
		}
		//free vector
		hdVector = VectorDestroy(hdVector);
		print("\n");
	}

	print("\n all done\n");

	cleanup_platform();
	return 0;
}
