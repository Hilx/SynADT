#include "sw_SynADT_malloc_wrapper.h"
#include "xio.h"

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
		token =  XIo_In32(MMU_BASE + MMU_TOKEN);
	}

	// write request
	XIo_Out32(MMU_BASE + MMU_CMD, size);

	// checking allocation status
	while(status == 0){
		status =  XIo_In32(MMU_BASE + MMU_STATUS);
	}

	//read result back
	result = XIo_In32(MMU_BASE + MMU_RESULT);

	return result;
}

void hw_mfree(int address){
	//print("Free\n");
	int token,status,result;
	token = 0;
	status = 0;

	// acquiring token
	while(token == 0){
		token =  XIo_In32(MMU_BASE + MMU_TOKEN);
	}

	// write request
	XIo_Out32(MMU_BASE + MMU_FREE, address);

	// checking de-allocation status
	while(status == 0){
		status =  XIo_In32(MMU_BASE + MMU_STATUS);
	}

	// performing a READ to reset token
	result = XIo_In32(MMU_BASE + MMU_RESULT);
	//print("Done Free\n");
}

void init_ddr(void){
	int i;
	for(i = 0; i <2396745; i++)
	XIo_Out32(0x18000000 + 4*i, 0);

}
