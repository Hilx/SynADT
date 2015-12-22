#include <stdio.h>
#include <stdlib.h>

#define RNG_SEED_INIT_X 123456789
#define RNG_SEED_INIT_Y 362436069
#define RNG_SEED_INIT_W 88675123

int seed_x = 123456789;
int seed_y = 362436069;
int seed_w = 88675123;

int RandGen(int log2_num_range);
unsigned MaskCreate(int NumOfBits);
int xorshift64(void);
void xor_rng_init(void);
