#include "top.h"

int CVectors(data_t *SlaveAXI, data_t *Master2Mem, data_t *Master2SysAlloc, volatile int input){

	#pragma HLS INTERFACE s_axilite port=return bundle=SlavePort
	#pragma HLS INTERFACE s_axilite port=SlaveAXI bundle=SlavePort
	#pragma HLS INTERFACE s_axilite port=input bundle=SlavePort
	#pragma HLS INTERFACE m_axi depth=1 port=Master2Mem offset=off
	#pragma HLS INTERFACE m_axi depth=1 port=Master2SysAlloc offset=off

	ptr_t hdVector;

	hdVector = VectorCreate(Master2Mem, Master2SysAlloc);

	if(input > 0){
		hdVector = VectorInit(Master2Mem, Master2SysAlloc, hdVector, 4);
	}

	if(input > 1){
		hdVector = VectorPushBack(Master2Mem, Master2SysAlloc, hdVector, 25);
	}

	if(input > 2){
		VectorPopBack(Master2Mem, hdVector);
	}

	if(input > 3){
		hdVector = VectorInsert(Master2Mem, Master2SysAlloc, hdVector, 8, 11);
	}

	if(input > 4){
		hdVector = VectorInsert(Master2Mem, Master2SysAlloc, hdVector, 12, 13);
	}

	if(input > 5){
		VectorDelete(Master2Mem, hdVector, 4);
	}

	if(input > 6){
		hdVector = VectorDestroy(Master2SysAlloc, hdVector);
	}


	return hdVector;
}
