#include <stdio.h>
#include <stdlib.h>
#include "sw_SynADT.h"

/* Counter */
#define COUNTER_BASE 0x43C20000 // change according to system
#define START 1
#define STOP 2
#define RESET 3

int *TreeGen(int NumberOfNodes, int *root);

/* Performance Measurements */
int *PM_1_INSERTION(int *root, int log2_tree_size);
int *PM_2_CHECK_INSERTION(int *root, int log2_tree_size);
int *PM_3_UPDATE(int *root, int log2_tree_size);
int *PM_4_DELETION(int *root);

/* software only */
void printTree(int *tree);
void printValue(int *tree);
