#include "rng4testing.h"

int RandGen(int log2_num_range){	
	unsigned mask;
	mask = MaskCreate(log2_num_range);
	return xorshift64() & mask;
}
 unsigned MaskCreate(int NumOfBits){
	/* input NumOfBits range 1 ~ 20 */	
	unsigned mask = 0;	
	int i;
	for(i = 0; i < NumOfBits; i++){
		mask |= 1 << i;
	}	
	return mask;	
}
int xorshift64(void){
	
	int t = seed_x ^( seed_x << 2);
	seed_x = seed_y;
	seed_y= seed_w;
	
	return seed_w = seed_w ^ (seed_w >> 13) ^ t ^ (t >> 23);
}

void xor_rng_init(void){
		seed_x = RNG_SEED_INIT_X;
		seed_y = RNG_SEED_INIT_Y;
		seed_w = RNG_SEED_INIT_W;
}
