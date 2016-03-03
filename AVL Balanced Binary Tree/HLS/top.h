#include <stdio.h>
#include <stdlib.h>
#include "SynADT.h"

#define USE_RANDOM_KEYS 1
#define USE_SEQUENTIAL_KEYS 2
int TestCase;

/* Performance Measurements */
ptr_t PM_1_INSERTION(data_t *myHeap, data_t *Master2SysAlloc, int *fixedStack, int stackPtr_avl, int log2_tree_size);
ptr_t PM_2_CHECK_INSERTION(data_t *myHeap, data_t *Master2SysAlloc, int *fixedStack, int stackPtr_avl, ptr_t root, int log2_tree_size);
ptr_t PM_3_UPDATE(data_t *myHeap, data_t *Master2SysAlloc, int *fixedStack, int *secondStack, int stackPtr_avl, ptr_t root, int log2_tree_size);
ptr_t PM_4_DELETION(data_t *myHeap, data_t *Master2SysAlloc, int *fixedStack, int stackPtr_avl, ptr_t root);

ptr_t add_new(data_t *myHeap, data_t *Master2SysAlloc, int *fixedStack, int stackPtr, ptr_t root, int key);

/* counter stuff */
//address
#define COUNTER_BASE 0x43C10000 // <- change this according to your system
//commands
#define C_START 1
#define C_STOP 2
#define C_RESET 3 // probably only needed in ARM core to reset the counter after the result has been read
