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


/* Performance Measurements */
ptr_t PM_1_INSERTION(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t stackPtr, ptr_t root, int log2_tree_size);
ptr_t PM_2_CHECK_INSERTION(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t stackPtr, ptr_t root, int log2_tree_size);
ptr_t PM_3_UPDATE(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t stackPtr, ptr_t root, int log2_tree_size);
ptr_t PM_4_DELETION(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t stackPtr, ptr_t treePtr);

#define USE_RAMDOM_KEYS 1
#define USE_SEQUENTIAL_KEYS 2
int TestCase;


#endif
