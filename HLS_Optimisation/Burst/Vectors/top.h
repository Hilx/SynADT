#ifndef TOP_H_
#define TOP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "SynADT.h"
/* counter stuff */
//address
#define COUNTER_BASE 0x43C10000 // <- change this according to your system
//commands
#define C_START 1
#define C_STOP 2
#define C_RESET 3 // probably only needed in ARM core to reset the counter after the result has been read

ptr_t PM1_INSERT(data_t *Master2Mem, data_t *Master2SysAlloc, int log2size, int *buffPtr);
ptr_t PM2_SEQ_ACCESS(data_t *Master2Mem, ptr_t hdVector, int log2size);
ptr_t PM3_RANDOM_ACCESS(data_t *Master2Mem, ptr_t hdVector, int log2size);

#endif
