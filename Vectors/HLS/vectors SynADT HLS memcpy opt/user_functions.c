#include "top.h"

ptr_t PM1_INSERT(data_t *Master2Mem, data_t *Master2SysAlloc, int log2size, int *buffPtr){
	ptr_t hdVector = VectorCreate(Master2Mem, Master2SysAlloc);
	int iter = (1 << log2size) - 2;

	int i;
	for(i = 0; i < iter; i++){
		hdVector = VectorPushBack(Master2Mem, Master2SysAlloc, hdVector, i, buffPtr);		
	}
	return hdVector+1;
}

ptr_t PM2_SEQ_ACCESS(data_t *Master2Mem, ptr_t hdVector, int log2size){
	int iter = 1 << (log2size-1);
	int i;
	int temp;
	for(i = 0; i < iter; i++){
		temp = vector_read_data(Master2Mem, hdVector, i);
	}
	return hdVector+1;
}

ptr_t PM3_RANDOM_ACCESS(data_t *Master2Mem, ptr_t hdVector, int log2size){
	int iter = 1 << (log2size-1);
	int i;
	int temp;
	for(i = 0; i < iter; i++){
		int index = RandGen(log2size) - 2;
		if(index < 0){
			index = 0;
		}
		temp = vector_read_data(Master2Mem, hdVector, index);
	}
	return hdVector +1;
}
