#include <stdio.h>
#include <stdlib.h>

#include "SynADT.h"

// Counter
#define COUNTER_BASE 0x43C10000
#define C_START 1
#define C_STOP 2
#define C_RESET 3


/* Performance Measurements */
ptr_t PM_1_INSERTION(data_t *myHeap, data_t *Master2SysAlloc, data_t *fixedStack, ptr_t stackPtr_avl, int log2_tree_size);
ptr_t PM_2_CHECK_INSERTION(data_t *myHeap, data_t *Master2SysAlloc, data_t *fixedStack, ptr_t stackPtr_avl, ptr_t root, int log2_tree_size);
ptr_t PM_3_UPDATE(data_t *myHeap, data_t *Master2SysAlloc, data_t *fixedStack, data_t *secondStack, ptr_t stackPtr_avl, ptr_t root, int log2_tree_size);
ptr_t PM_4_DELETION(data_t *myHeap, data_t *Master2SysAlloc, data_t *fixedStack, ptr_t stackPtr_avl, ptr_t root);


#define USE_RANDOM_KEYS 1
#define USE_SEQUENTIAL_KEYS 2
int TestCase;

int structure_size;
