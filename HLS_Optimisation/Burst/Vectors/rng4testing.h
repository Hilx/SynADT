#include <stdio.h>
#include <stdlib.h>

struct seed_t{
	int x,y,w;
};

int x = 123456789;
int y = 362436069;
int w = 88675123;

int RandGen(int log2_num_range);
unsigned MaskCreate(int NumOfBits);
int xorshift64(void);
