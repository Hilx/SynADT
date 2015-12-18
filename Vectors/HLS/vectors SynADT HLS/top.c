#include "top.h"

int CVectors(data_t *SlaveAXI, data_t *Master2Mem, data_t *Master2SysAlloc, data_t input_n){

	#pragma HLS INTERFACE s_axilite port=return bundle=SlavePort
	#pragma HLS INTERFACE s_axilite port=SlaveAXI bundle=SlavePort
	#pragma HLS INTERFACE s_axilite port=input_n bundle=SlavePort
	#pragma HLS INTERFACE m_axi depth=1 port=Master2Mem offset=off
	#pragma HLS INTERFACE m_axi depth=1 port=Master2SysAlloc offset=off

	/* -- Control -- */
	int log2size = input_n;
	
	/* -- Functions -- */
	
	ptr_t hdVector;
	ptr_t hdVector_old;
	
	hdVector = NULL_PTR;
	Master2SysAlloc[(COUNTER_BASE)/4 + 1] = C_START;
	hdVector_old = hdVector;
	hdVector =  PM1_INSERT(Master2Mem, Master2SysAlloc, log2size);
	if(hdVector != hdVector_old){
		Master2SysAlloc[(COUNTER_BASE)/4 + 1] = C_STOP;
		hdVector--;
	}
	

	Master2SysAlloc[(COUNTER_BASE)/4 + 2] = C_START;
	hdVector_old = hdVector;
	hdVector = PM2_SEQ_ACCESS(Master2Mem, hdVector, log2size);
	if(hdVector != hdVector_old){
		Master2SysAlloc[(COUNTER_BASE)/4 + 2] = C_STOP;
		hdVector--;
	}

	Master2SysAlloc[(COUNTER_BASE)/4 + 3] = C_START;
	hdVector_old = hdVector;
	hdVector = PM3_RANDOM_ACCESS(Master2Mem, hdVector, log2size);
	if(hdVector != hdVector_old){
		Master2SysAlloc[(COUNTER_BASE)/4 + 3] = C_STOP;
		hdVector --;
	}

	return hdVector;
}
