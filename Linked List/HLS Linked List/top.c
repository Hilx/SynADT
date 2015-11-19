#include "top.h"

int ListIP(data_t *SlaveAXI, data_t *Master2Mem, data_t *Master2SysAlloc){

	#pragma HLS INTERFACE s_axilite port=SlaveAXI bundle=SlavePort
	#pragma HLS INTERFACE s_axilite port=return   bundle=SlavePort
	#pragma HLS INTERFACE m_axi depth=1 port=Master2Mem offset=off
	#pragma HLS INTERFACE m_axi depth=1 port=Master2SysAlloc offset=off

	ptr_t hdPtr = NULL_PTR;
	ptr_t hdPtr_old;
	int SizeOfList = 1310720;
	
	Master2SysAlloc[(COUNTER_BASE)/4] = C_START;

	/* Creating a list */
	Master2SysAlloc[(COUNTER_BASE)/4 + 1] = C_START;
	hdPtr_old = hdPtr;
	hdPtr = ListGen(Master2Mem, Master2SysAlloc, SizeOfList, hdPtr);
	if(hdPtr != hdPtr_old){
		Master2SysAlloc[(COUNTER_BASE)/4 + 1] = C_STOP;
	}


	/* Reverse the list */
	Master2SysAlloc[(COUNTER_BASE)/4 + 2] = C_START;
	hdPtr_old = hdPtr;
	hdPtr = ReverseList(Master2Mem, hdPtr);
	if(hdPtr != hdPtr_old){
		Master2SysAlloc[(COUNTER_BASE)/4 + 2] = C_STOP;
	}

	/* Delete the list */
	Master2SysAlloc[(COUNTER_BASE)/4 + 3] = C_START;
	hdPtr_old = hdPtr;
	hdPtr = DeleteList(Master2Mem, Master2SysAlloc, hdPtr);
	if(hdPtr != hdPtr_old){
		Master2SysAlloc[(COUNTER_BASE)/4 + 3] = C_STOP;
	}


	Master2SysAlloc[(COUNTER_BASE)/4] = C_STOP;

	return hdPtr;
}
