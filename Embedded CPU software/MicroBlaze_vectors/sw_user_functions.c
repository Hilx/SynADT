#include "sw_top.h"

int *PM1_INSERT(int log2size){
	int *hdVector = VectorCreate();
	int iter = (1 << log2size) - 2;

	int i;
//	print("\n");
	for(i = 0; i < iter; i++){
	//	putnum(i);
	//	print(" ");
		hdVector = VectorPushBack(hdVector, i);		
	//	putnum(hdVector);
	//	print("\n");
	}
	return hdVector;
}

void PM2_SEQ_ACCESS(int *hdVector, int log2size){
	int iter = 1 << (log2size-1);
	//printf("\nPM2: ");
	int i;
	for(i = 0; i < iter; i++){
		int temp = vector_read_data(hdVector, i);
		//printf("%d, ",temp);
	}
	//printf("\n\n");
}

void PM3_RANDOM_ACCESS(int *hdVector, int log2size){
	int iter = 1 << (log2size-1);
	//printf("PM3: ");
	int i;
	for(i = 0; i < iter; i++){
		int index = RandGen(log2size) - 2;
		if(index < 0){
			index = 0;
		}
		int temp = vector_read_data(hdVector, index);
		//printf("%d, ",temp);
	}
	//printf("\n");
}
