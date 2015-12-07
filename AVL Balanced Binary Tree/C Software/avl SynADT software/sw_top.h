#include <stdio.h>
#include <stdlib.h>
#include "sw_SynADT.h"

int *TreeGen(int NumberOfNodes, int *root);

/* Performance Measurements */
int *PM_1_INSERTION(int *root, int log2_tree_size);
int *PM_2_CHECK_INSERTION(int *root, int log2_tree_size);
int *PM_3_UPDATE(int *root, int log2_tree_size);
int *PM_4_DELETION(int *root);

/* software only */
void printTree(int *tree);
void printValue(int *tree);