#include "top.h"

int CList(data_t *SlaveAXI, data_t *Master2Mem, data_t *Master2SysAlloc, data_t input_r){

	#pragma HLS INTERFACE s_axilite port=input_r bundle=SlavePort
	#pragma HLS INTERFACE s_axilite port=SlaveAXI bundle=SlavePort
	#pragma HLS INTERFACE s_axilite port=return   bundle=SlavePort
	#pragma HLS INTERFACE m_axi depth=1 port=Master2Mem offset=off
	#pragma HLS INTERFACE m_axi depth=1 port=Master2SysAlloc offset=off
	// for burst
	int burst_buffer[2];
	int *buffPtr = burst_buffer;
	
	int log2SizeOfList = input_r;
	ptr_t hdPtr = NULL_PTR;
	ptr_t hdPtr_old;

	Master2SysAlloc[(COUNTER_BASE)/4 + 1] = C_START;
	hdPtr_old = hdPtr;
	hdPtr =  PM_RandListGen(Master2Mem, Master2SysAlloc, log2SizeOfList, hdPtr, buffPtr);
	if(hdPtr != hdPtr_old){
		Master2SysAlloc[(COUNTER_BASE)/4 + 1] = C_STOP;
		hdPtr--;
	}

	Master2SysAlloc[(COUNTER_BASE)/4 + 2] = C_START;
	hdPtr_old = hdPtr;
	hdPtr = PM_ReverseList( Master2Mem, hdPtr);
	if(hdPtr != hdPtr_old){
		Master2SysAlloc[(COUNTER_BASE)/4 + 2] = C_STOP;
		hdPtr--;
	}

	Master2SysAlloc[(COUNTER_BASE)/4 + 3] = C_START;
	hdPtr_old = hdPtr;
	hdPtr =  PM_DeleteList( Master2Mem, Master2SysAlloc, hdPtr);
	if(hdPtr != hdPtr_old){
		Master2SysAlloc[(COUNTER_BASE)/4 + 3] = C_STOP;
		hdPtr--;
	}

	return hdPtr;
}
