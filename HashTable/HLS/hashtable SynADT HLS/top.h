#ifndef TOP_H_
#define TOP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "SynADT.h"

/* Counter */
#define COUNTER_BASE 0x43C10000 
#define C_START 1
#define C_STOP 2
#define C_RESET 3

/* Test Case */
#define USE_RAMDOM_KEYS 1
#define USE_SEQUENTIAL_KEYS 2
int TestCase;

#define LF_THRESHOLD 7
#define LOG2_CHAIN 4	

/* User Functions */
ptr_t PM_1_INSERTION(data_t *Master2Mem, data_t *Master2SysAlloc, int log2size);
ptr_t PM_2_CHECK_INSERTION(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdTable, int log2size);
ptr_t PM_3_UPDATE(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdTable, int log2size);
ptr_t PM_4_DELETION(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdTable);
void table_init(int LOG2_TOTAL);

#endif