#include "sw_SynADT.h"

int *VectorInit(int *hdVector, int size){
	int i;
	for(i = 0; i<size; i++){
		hdVector = VectorPushBack(hdVector, i*4);
	}
	return hdVector;
}

/* vectors */
int VectorCheckFit(int *hdVector, int maxVolumn, int currentVecSize){
	if(currentVecSize == maxVolumn){
		return FIT_NO;
	}else{
		return FIT_OK;
	}	
}

int *VectorCreate(void){
	int hls_request_size = 28;	//2^5-4
	int req_size = hls_request_size/4; 
	int *hdVector = SysMalloc(hls_request_size);
	int maxVolumn = req_size - 2;
	vector_write_volumn(hdVector, maxVolumn);
	vector_write_size(hdVector, 0);
	return hdVector;
}

int *VectorRealloc(int maxVolumn, int currentVecSize){
	int hls_request_size = ((maxVolumn +2) *4 +4)*2 - 4;
	int req_size = hls_request_size/4;
	int volumn = (maxVolumn + 2) * 2; 
	int *newVectorPtr = SysMalloc(hls_request_size); 
	maxVolumn = req_size - 2;
	vector_write_volumn(newVectorPtr, maxVolumn);
	return newVectorPtr;
}

int *VectorPushBack(int *hdVector, int data){
	int maxVolumn = vector_read_volumn(hdVector);
	int currentVecSize = vector_read_size(hdVector);
	
	int ifFit = VectorCheckFit(hdVector, maxVolumn, currentVecSize);
	
	if( ifFit == FIT_OK){
		//Insert
		vector_write_data(hdVector, currentVecSize, data);
		vector_write_size(hdVector, currentVecSize +1);
		return hdVector;
	}else{
		//Re-alloc
		int *hdVector_new = VectorRealloc(maxVolumn, currentVecSize);
		int i;
		for(i = 0; i < currentVecSize; i++){
			vector_write_data(hdVector_new, i, vector_read_data(hdVector, i)); 
		}
		vector_write_data(hdVector_new, currentVecSize, data);
		vector_write_size(hdVector_new, currentVecSize +1);		
		return hdVector_new;
	}
}

void VectorPopBack(int *hdVector){
	int currentVecSize = vector_read_size(hdVector);
	if(currentVecSize != 0){
		vector_write_size(hdVector, currentVecSize - 1);
	}
}

int *VectorInsert(int *hdVector, int keyData, int newData){
	int maxVolumn = vector_read_volumn(hdVector);
	int currentVecSize = vector_read_size(hdVector);
	int *hdVector_new;
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
			checkData = vector_read_data(hdVector, i);
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
			origData = vector_read_data(hdVector, j);
			vector_write_data(hdVector, j, writeData);
			writeData = origData;
		}
		vector_write_data(hdVector, currentVecSize, writeData);
		vector_write_size(hdVector, currentVecSize + 1);
		
		return hdVector;
	}else{
		//Re-alloc
		hdVector_new = VectorRealloc(maxVolumn, currentVecSize);

		// search
		i = 0;
		while(flag_found == 0){
			
			checkData = vector_read_data(hdVector, i);	
			vector_write_data(hdVector_new, i, checkData);
			if(checkData == keyData){
				flag_found = 1;
			}else{
				i++;
			}
		}
		// write and move other data down by one slot
		int j;
		writeData = newData;
		for(j = i + 1; j < currentVecSize; j++){
			origData = vector_read_data(hdVector, j);
			vector_write_data(hdVector_new, j, writeData);
			writeData = origData;
		}
		vector_write_data(hdVector_new, currentVecSize, writeData);		
		vector_write_size(hdVector_new, currentVecSize + 1);		
		return hdVector_new;
	}	
}

void VectorDelete(int *hdVector, int keyData){
	int currentVecSize = vector_read_size(hdVector);
	// search
	int i = 0;
	int checkData, writeData;
	int flag_found = 0;
	while(flag_found == 0){
		checkData = vector_read_data(hdVector, i);
		if(checkData == keyData){
			flag_found = 1;
		}else{
			i++;
		}
	}
	// move some data up by one slot
	int j;
	for(j = i + 1; j < currentVecSize; j++){
		writeData = vector_read_data(hdVector, j);
		vector_write_data(hdVector, j - 1, writeData);
	}
	
	vector_write_size(hdVector, currentVecSize - 1);	
}

int *VectorDestroy(int *hdVector){
	SysFree(hdVector);
	return NULL;
}

/* memory access functions */
int vector_read_volumn(int *hdVector){
	return hdVector[VECTOR_VOLUMN];
}
int vector_read_size(int *hdVector){
	return hdVector[VECTOR_SIZE];	
}
int vector_read_data(int *hdVector, int index){
	return hdVector[VECTOR_CONTENT + index];
}

void vector_write_volumn(int *hdVector, int volumn){
	hdVector[VECTOR_VOLUMN] = volumn;
}
void vector_write_size(int *hdVector, int index){
	hdVector[VECTOR_SIZE] = index;	
}
void vector_write_data(int *hdVector, int index, int data){
	hdVector[VECTOR_CONTENT + index] = data;
}

/* software only */
void PrintVec(int *hdVector){
	printf("printing vector: [");
	int vecSize = vector_read_size(hdVector);
	int i;
	int data;
	if(hdVector == NULL || vector_read_size(hdVector) == 0){
		
		printf(" empty ]\n");
	}
	else{
		
		for(i = 0; i < vecSize; i++){
			data = vector_read_data(hdVector, i);
			printf(" %d", data);
		}
		printf(" ]\n");
	}
}
