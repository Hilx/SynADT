#include "sw_SynADT_malloc_wrapper.h"

/*  Malloc & free Wrapper */
int *SysMalloc(int size){
	int *nowPtr;
	//nowPtr = (int*)malloc(size);
	nowPtr = hw_malloc(size);
	return nowPtr;
}

void SysFree(int *nowPtr){
	//free(nowPtr);
	hw_mfree(nowPtr);
}

int hw_malloc(int size){
	int token,status,result;
	token = 0;
	status = 0;

	// acquiring token
	while(token == 0){
		token =  Xil_In32(MMU_BASE + MMU_TOKEN);
	}

	// write request
	Xil_Out32(MMU_BASE + MMU_CMD, size);

	// checking allocation status
	while(status == 0){
		status =  Xil_In32(MMU_BASE + MMU_STATUS);
	}

	//read result back
	result = Xil_In32(MMU_BASE + MMU_RESULT);

	return result;
}

void hw_mfree(int address){
	//print("Free\n");
	int token,status,result;
	token = 0;
	status = 0;

	// acquiring token
	while(token == 0){
		token =  Xil_In32(MMU_BASE + MMU_TOKEN);
	}

	// write request
	Xil_Out32(MMU_BASE + MMU_FREE, address);

	// checking de-allocation status
	while(status == 0){
		status =  Xil_In32(MMU_BASE + MMU_STATUS);
	}

	// performing a READ to reset token
	result = Xil_In32(MMU_BASE + MMU_RESULT);
	//print("Done Free\n");
}

void SysAlloc_init(void){
	Xil_Out32(MMU_BASE + MMU_INIT_RANGE, 2396745 - 23768);
	Xil_Out32(MMU_BASE + MMU_INIT,1);
	int init_done = 0;
	while(init_done == 0){
		init_done = Xil_In32(MMU_BASE + MMU_INIT);
	}
	print(" (Init Done) ");
}

