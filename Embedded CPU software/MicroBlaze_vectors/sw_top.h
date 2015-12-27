#include <stdio.h>
#include <stdlib.h>
#include "sw_SynADT.h"
/* Counter */
#define COUNTER_BASE 0x43C10000 // change according to system
#define START 1
#define STOP 2
#define RESET 3

int *PM1_INSERT(int log2size);
void PM2_SEQ_ACCESS(int *hdVector, int log2size);
void PM3_RANDOM_ACCESS(int *hdVector, int log2size);
