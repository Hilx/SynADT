#include "rng4testing.h"
/*
int main(void){
	
	printf("What's up RNG!\n");

	int i;
	
	printf("insert()\n");
	printf("x = %d, y = %d, w = %d \n", x, y, w);	
	for(i = 0; i < 8; i++){
		printf("%d\n",RandGen(4));
	}
	
	printf("check then insert()\n");
	printf("x = %d, y = %d, w = %d \n", x, y, w);
	for(i = 0; i < 8; i++){
		printf("%d\n",RandGen(5));
	}
	
	printf("updates()\n");
	x = 123456789;
	y = 362436069;
	w = 88675123;
	printf("x = %d, y = %d, w = %d \n", x, y, w);
	for(i = 0; i < 8; i++){
		printf("%d %d\n",RandGen(4), RandGen(5));
	}
}
*/
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
	seed_y = seed_w;
	
	return seed_w = seed_w ^ (seed_w >> 13) ^ t ^ (t >> 23);
}