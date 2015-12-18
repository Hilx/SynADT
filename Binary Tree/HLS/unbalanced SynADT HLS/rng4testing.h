#include <stdio.h>
#include <stdlib.h>

#define RNG_SEED_INIT_X 123456789
#define RNG_SEED_INIT_Y 362436069
#define RNG_SEED_INIT_W 88675123

int seed_x = RNG_SEED_INIT_X;
int seed_y = RNG_SEED_INIT_Y;
int seed_w = RNG_SEED_INIT_W;


int RandGen(int log2_num_range);
unsigned MaskCreate(int NumOfBits);
int xorshift64(void);
void xor_rng_init(void);
