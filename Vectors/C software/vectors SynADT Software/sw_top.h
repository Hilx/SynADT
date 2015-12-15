#include <stdio.h>
#include <stdlib.h>
#include "sw_SynADT.h"

int *PM1_INSERT(int log2size);
void PM2_SEQ_ACCESS(int *hdVector, int log2size);
void PM3_RANDOM_ACCESS(int *hdVector, int log2size);