#ifndef TOP_H_
#define TOP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "SynADT.h"

/* counter stuff */
//address
#define COUNTER_BASE 0x43C20000 // <- change this according to your system
//commands
#define C_START 1
#define C_STOP 2
#define C_RESET 3 // probably only needed in ARM core to reset the counter after the result has been read

ptr_t ListGen(data_t *Master2Mem, data_t *Master2SysAlloc, int SizeOfList, ptr_t hdPtr);

/* Performance Measurements */
/* Part 1 */
ptr_t PM_RandListGen(data_t *Master2Mem, data_t *Master2SysAlloc, int log2SizeOfList, ptr_t hdPtr);
/* Part 2 */
ptr_t PM_ReverseList(data_t *Master2Mem,ptr_t hdPtr);
/* Part 3 */
ptr_t PM_DeleteList(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdPtr);

#endif
