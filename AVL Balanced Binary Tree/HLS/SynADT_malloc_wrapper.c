#include "SynADT_malloc_wrapper.h"

volatile int SysMalloc(int size, data_t *Master2SysAlloc){

	int token = 0, status = 0, result = 0;

	while(token == 0){
    	token = Master2SysAlloc[(MMU_BASE + MMU_TOKEN)/4];
	}

	Master2SysAlloc[(MMU_BASE+ MMU_CMD)/4] = size;

	while(status == 0){
		status = Master2SysAlloc[(MMU_BASE+ MMU_STATUS)/4];
	}

	result = Master2SysAlloc[(MMU_BASE+ MMU_RESULT)/4];

	return result;
}

volatile void SysFree(int addr, data_t *Master2SysAlloc){

	int token = 0, status = 0, result = 0;

	while(token == 0){
    	token = Master2SysAlloc[(MMU_BASE + MMU_TOKEN)/4];
	}

	Master2SysAlloc[(MMU_BASE+ MMU_FREE)/4] = addr;

	while(status == 0){
		status = Master2SysAlloc[(MMU_BASE+ MMU_STATUS)/4];
	}

	result = Master2SysAlloc[(MMU_BASE+ MMU_RESULT)/4];
}