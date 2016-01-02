#ifndef TOP_H_
#define TOP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "SynADT.h"

/* counter stuff */
#define COUNTER_BASE 0x43C10000

#define C_START 1
#define C_STOP 2
#define C_RESET 3

/* Performance Measurements */
ptr_t PM_RandListGen(data_t *Master2Mem, data_t *Master2SysAlloc, int log2SizeOfList, ptr_t hdPtr, int *buffPtr);
ptr_t PM_ReverseList(data_t *Master2Mem,ptr_t hdPtr);
ptr_t PM_DeleteList(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdPtr);

#endif
