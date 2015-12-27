#include <stdio.h>
#include <stdlib.h>
#include "sw_SynADT.h"

/* Counter */
#define COUNTER_BASE 0x43C10000 // change according to system
#define START 1
#define STOP 2
#define RESET 3

#define USE_RANDOM_KEYS 1
#define USE_SEQUENTIAL_KEYS 2
int TestCase;

#define LF_THRESHOLD 7
#define LOG2_CHAIN 4	

int *PM_1_INSERTION(int log2_tree_size);
void PM_2_CHECK_INSERTION(int *hdTable, int log2_tree_size);
void PM_3_UPDATE(int *hdTable, int log2_tree_size);
void PM_4_DELETION(int *hdTable);
void table_init(int LOG2_TOTAL);
