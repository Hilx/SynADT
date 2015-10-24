#include "main.h"

/* SYSALLOC FUNCTIONS */
int SysMalloc(int size){
	int index = myHeapIndex;
	myHeapIndex = myHeapIndex + NODE_SIZE;
	return index;
}
void SysFree(int addr){
	NULL;
}
