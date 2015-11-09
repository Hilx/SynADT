#include "hashtable.h"

/* SYSALLOC FUNCTIONS */
int SysMalloc(int size){

	int index = myHeapIndex;
	myHeapIndex = myHeapIndex + size;
	return index;
}
void SysFree(int addr){

	NULL;
}
