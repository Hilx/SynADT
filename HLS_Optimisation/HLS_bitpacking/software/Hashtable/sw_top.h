#include <stdio.h>
#include <stdlib.h>
#include "sw_SynADT.h"

#define USE_RANDOME_KEYS 1
#define USE_SEQUENTIAL_KEYS 2
int TestCase;

#define LF_THRESHOLD 7
#define LOG2_CHAIN 4	

int *PM_1_INSERTION(int log2_tree_size);
void PM_2_CHECK_INSERTION(int *hdTable, int log2_tree_size);
void PM_3_UPDATE(int *hdTable, int log2_tree_size);
void PM_4_DELETION(int *hdTable);
void table_init(int LOG2_TOTAL);