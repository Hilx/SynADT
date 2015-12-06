#include "sw_SynADT.h"

/* Counter */
#define COUNTER_BASE 0x43C10000 // change according to system
#define START 1
#define STOP 2
#define RESET 3

int *ListGen(int SizeOfList, int *hdPtr);
int *RandListGen(int log2SizeOfList, int *hdPtr);
void PrintList(int *hdPtr);
