#include <stdio.h>
#include <stdlib.h>

#include "SynADT.h"

/* Performance Measurements */
ptr_t PM_1_INSERTION(int *myHeap, int *fixedStack, int stackPtr_avl, int log2_tree_size);
ptr_t PM_2_CHECK_INSERTION(int *myHeap, int *fixedStack, int stackPtr_avl, ptr_t root, int log2_tree_size);
ptr_t PM_3_UPDATE(int *myHeap, int *fixedStack, int *secondStack, int stackPtr_avl, ptr_t root, int log2_tree_size);
ptr_t PM_4_DELETION(int *myHeap, int *fixedStack, int stackPtr_avl, ptr_t root);

/* Software Only */
void printTree(int *myHeap, ptr_t nowPtr);
void printValue(int *myHeap, ptr_t nowPtr);

#define USE_RANDOM_KEYS 1
#define USE_SEQUENTIAL_KEYS 2
int TestCase;
