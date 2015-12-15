#include "sw_top.h"

int main(){
	
	int log2size = 5;
	
	xor_rng_init();
	
	int *hdVector = PM1_INSERT(log2size);
	PM2_SEQ_ACCESS(hdVector, log2size);
	PM3_RANDOM_ACCESS(hdVector, log2size);
}
