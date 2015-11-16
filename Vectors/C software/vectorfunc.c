#include "main.h"

ptr_t VectorInit(int *myHeap, ptr_t hdVector, int size){
	int i;
	ptr_t outPtr;
	for(i = 0; i<size; i++){
		outPtr = VectorPushBack(myHeap, hdVector, i*4);
	}
	return outPtr;
}

/* vectors */
int VectorCheckFit(ptr_t hdVector, int maxVolumn, int currentVecSize){
	if(currentVecSize == maxVolumn){
		return FIT_NO;
	}else{
		return FIT_OK;
	}	
}

ptr_t VectorCreate(int *myHeap){
	int hls_request_size = 28;	//2^5-4
	int req_size = hls_request_size/4; 
	ptr_t hdVector = SysMalloc(req_size);// hls_request_size for hls
	int maxVolumn = req_size - 2;
	vector_write_volumn(myHeap, hdVector, maxVolumn);
	vector_write_size(myHeap, hdVector, 0);
	return hdVector;
}

ptr_t VectorRealloc(int *myHeap, int maxVolumn, int currentVecSize){
	int hls_request_size = ((maxVolumn +2) *4 +4)*2 - 4;
	int req_size = hls_request_size/4;
	int volumn = (maxVolumn + 2) * 2; 
	ptr_t newVectorPtr = SysMalloc(req_size); // *Change for HLS
	maxVolumn = req_size - 2;
	vector_write_volumn(myHeap, newVectorPtr, maxVolumn);
	return newVectorPtr;
}

ptr_t VectorPushBack(int *myHeap, ptr_t hdVector, int data){
	int maxVolumn = vector_read_volumn(myHeap, hdVector);
	int currentVecSize = vector_read_size(myHeap, hdVector);
	ptr_t hdVector_new;
	int ifFit = VectorCheckFit(hdVector, maxVolumn, currentVecSize);
	
	if( ifFit == FIT_OK){
		//Insert
		vector_write_data(myHeap, hdVector, currentVecSize, data);
		vector_write_size(myHeap, hdVector, currentVecSize +1);
		return hdVector;
	}else{
		//Re-alloc
		hdVector_new = VectorRealloc(myHeap, maxVolumn, currentVecSize);
		int i;
		for(i = 0; i < currentVecSize; i++){
			vector_write_data(myHeap, hdVector_new, i, vector_read_data(myHeap, hdVector, i)); 
		}
		vector_write_data(myHeap, hdVector_new, currentVecSize, data);
		vector_write_size(myHeap, hdVector_new, currentVecSize +1);		
		SysFree(hdVector);
		return hdVector_new;
	}
}

void VectorPopBack(int *myHeap, ptr_t hdVector){
	int currentVecSize = vector_read_size(myHeap, hdVector);
	if(currentVecSize != 0){
		vector_write_size(myHeap, hdVector, currentVecSize - 1);
	}
}

ptr_t VectorInsert(int *myHeap, ptr_t hdVector, int keyData, int newData){
	int maxVolumn = vector_read_volumn(myHeap, hdVector);
	int currentVecSize = vector_read_size(myHeap, hdVector);
	ptr_t hdVector_new;
	int ifFit = VectorCheckFit(hdVector, maxVolumn, currentVecSize);
	int flag_found = 0;
	int i;
	int checkData;
	int origData, writeData;
	if( ifFit == FIT_OK){
		//Insert
		//Search for keyData's index
		i = 0;
		while(flag_found == 0){
			checkData = vector_read_data(myHeap, hdVector, i);
			if(checkData == keyData){
				flag_found = 1;
			}else{
				i++;
			}
		}
		// write new data, and move all other data one slot down
		int j;
		writeData = newData;
		for(j = i + 1; j < currentVecSize; j++){
			origData = vector_read_data(myHeap, hdVector, j);
			vector_write_data(myHeap, hdVector, j, writeData);
			writeData = origData;
		}
		vector_write_data(myHeap, hdVector, currentVecSize, writeData);
		vector_write_size(myHeap, hdVector, currentVecSize + 1);
		
		return hdVector;
	}else{
		//Re-alloc
		hdVector_new = VectorRealloc(myHeap, maxVolumn, currentVecSize);

		// search
		i = 0;
		while(flag_found == 0){
			checkData = vector_read_data(myHeap, hdVector, i);	
			if(checkData == keyData){
				flag_found = 1;
			}else{
				i++;
			}
			vector_write_data(myHeap, hdVector_new, i, checkData);
		}
		// write and move other data down by one slot
		int j;
		writeData = newData;
		for(j = i + 1; j < currentVecSize; j++){
			origData = vector_read_data(myHeap, hdVector, j);
			vector_write_data(myHeap, hdVector_new, j, writeData);
			writeData = origData;
		}
		vector_write_data(myHeap, hdVector_new, currentVecSize, writeData);		
		vector_write_size(myHeap, hdVector_new, currentVecSize + 1);		
		
		SysFree(hdVector);
		return hdVector_new;
	}	
}

void VectorDelete(int *myHeap, ptr_t hdVector, int keyData){
	int currentVecSize = vector_read_size(myHeap, hdVector);
	// search
	int i = 0;
	int checkData, writeData;
	int flag_found = 0;
	while(flag_found == 0){
		checkData = vector_read_data(myHeap, hdVector, i);
		if(checkData == keyData){
			flag_found = 1;
		}else{
			i++;
		}
	}
	// move some data up by one slot
	int j;
	for(j = i + 1; j < currentVecSize; j++){
		writeData = vector_read_data(myHeap, hdVector, j);
		vector_write_data(myHeap, hdVector, j - 1, writeData);
	}
	
	vector_write_size(myHeap, hdVector, currentVecSize - 1);	
}

ptr_t VectorDestroy(int *myHeap, ptr_t hdVector){
	SysFree(hdVector);
	return NULL_PTR;
}

/* memory access functions */
int vector_read_volumn(int *myHeap, ptr_t hdVector){
	return myHeap[hdVector + VECTOR_VOLUMN];
}
int vector_read_size(int *myHeap, ptr_t hdVector){
	return myHeap[hdVector + VECTOR_SIZE];	
}
int vector_read_data(int *myHeap, ptr_t hdVector, int index){
	return myHeap[hdVector + VECTOR_CONTENT + index];
}

void vector_write_volumn(int *myHeap, ptr_t hdVector, int volumn){
	myHeap[hdVector + VECTOR_VOLUMN] = volumn;
}
void vector_write_size(int *myHeap, ptr_t hdVector, int index){
	myHeap[hdVector + VECTOR_SIZE] = index;	
}
void vector_write_data(int *myHeap, ptr_t hdVector, int index, int data){
	myHeap[hdVector+ VECTOR_CONTENT + index] = data;
}

/* software only */
void PrintVec(int *myHeap, ptr_t hdVector){
	printf("printing vector: [");
	int vecSize = vector_read_size(myHeap, hdVector);
	int i;
	int data;
	if(hdVector == NULL_PTR || vector_read_size(myHeap, hdVector) == 0){
		
		printf(" empty ]\n");
	}
	else{
		
		for(i = 0; i < vecSize; i++){
			data = vector_read_data(myHeap, hdVector, i);
			printf(" %d", data);
		}
		printf(" ]\n");
	}
}