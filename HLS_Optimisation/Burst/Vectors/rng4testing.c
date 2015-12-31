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
	
	int t = x ^( x << 2);
	x = y;
	y= w;
	
	return w = w ^ (w >> 13) ^ t ^ (t >> 23);
}