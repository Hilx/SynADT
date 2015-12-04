#include "sw_SynADT_malloc_wrapper.h"

/*  Malloc & free Wrapper */
int *SysMalloc(int size){
	int *nowPtr;
	nowPtr = (int*)malloc(size);
	return nowPtr;
}

void SysFree(int *nowPtr){
	free(nowPtr);
}