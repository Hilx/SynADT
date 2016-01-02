#include "sw_SynADT_malloc_wrapper.h"

/*  Malloc & free Wrapper */
int *SysMalloc(int size){
	int *out = malloc(size);
	return out;
}

void SysFree(int *nowPtr){
	free(nowPtr);
}