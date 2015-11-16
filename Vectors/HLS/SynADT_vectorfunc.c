#include "SynADT_header.h"

ptr_t VectorInit(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdVector, int size){
	int i;
	ptr_t outPtr;
	for(i = 0; i<size; i++){
		outPtr = VectorPushBack(Master2Mem, Master2SysAlloc, hdVector, i*4);
	}
	return outPtr;
}

/* vectors */
int VectorCheckFit(data_t *Master2Mem, ptr_t hdVector, int maxVolumn, int currentVecSize){
	if(currentVecSize == maxVolumn){
		return FIT_NO;
	}else{
		return FIT_OK;
	}	
}

ptr_t VectorCreate(data_t *Master2Mem, data_t *Master2SysAlloc){
	data_t *myHeap = Master2Mem;
	int hls_request_size = 28;	//2^5-4
	int req_size = hls_request_size/4; 
	ptr_t hdVector = SysMalloc(hls_request_size, Master2SysAlloc);// hls_request_size for hls
	int maxVolumn = req_size - 2;
	vector_write_volumn(myHeap, hdVector, maxVolumn);
	vector_write_size(myHeap, hdVector, 0);
	return hdVector;
}

ptr_t VectorRealloc(data_t *Master2Mem, data_t *Master2SysAlloc, int maxVolumn, int currentVecSize){
	data_t *myHeap = Master2Mem;
	int hls_request_size = ((maxVolumn +2) *4 +4)*2 - 4;
	int req_size = hls_request_size/4;
	int volumn = (maxVolumn + 2) * 2; 
	ptr_t newVectorPtr = SysMalloc(hls_request_size, Master2SysAlloc); // *Change for HLS
	maxVolumn = req_size - 2;
	vector_write_volumn(myHeap, newVectorPtr, maxVolumn);
	return newVectorPtr;
}

ptr_t VectorPushBack(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdVector, int data){
	data_t *myHeap = Master2Mem;
	int maxVolumn = vector_read_volumn(myHeap, hdVector);
	int currentVecSize = vector_read_size(myHeap, hdVector);
	ptr_t hdVector_new;
	int ifFit = VectorCheckFit(myHeap, hdVector, maxVolumn, currentVecSize);
	
	if( ifFit == FIT_OK){
		//Insert
		vector_write_data(myHeap, hdVector, currentVecSize, data);
		vector_write_size(myHeap, hdVector, currentVecSize +1);
		return hdVector;
	}else{
		//Re-alloc
		hdVector_new = VectorRealloc(myHeap, Master2SysAlloc, maxVolumn, currentVecSize);
		int i;
		for(i = 0; i < currentVecSize; i++){
			vector_write_data(myHeap, hdVector_new, i, vector_read_data(myHeap, hdVector, i)); 
		}
		vector_write_data(myHeap, hdVector_new, currentVecSize, data);
		vector_write_size(myHeap, hdVector_new, currentVecSize +1);		
		SysFree(hdVector, Master2SysAlloc);
		return hdVector_new;
	}
}

void VectorPopBack(data_t *Master2Mem, ptr_t hdVector){
	data_t *myHeap = Master2Mem;
	int currentVecSize = vector_read_size(myHeap, hdVector);
	if(currentVecSize != 0){
		vector_write_size(myHeap, hdVector, currentVecSize - 1);
	}
}

ptr_t VectorInsert(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdVector, int keyData, int newData){
	data_t *myHeap = Master2Mem;
	int maxVolumn = vector_read_volumn(myHeap, hdVector);
	int currentVecSize = vector_read_size(myHeap, hdVector);
	ptr_t hdVector_new;
	int ifFit = VectorCheckFit(myHeap, hdVector, maxVolumn, currentVecSize);
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
		hdVector_new = VectorRealloc(myHeap, Master2SysAlloc, maxVolumn, currentVecSize);

		// search
		i = 0;
		while(flag_found == 0){
			checkData = vector_read_data(myHeap, hdVector, i);	
			vector_write_data(myHeap, hdVector_new, i, checkData);
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
			origData = vector_read_data(myHeap, hdVector, j);
			vector_write_data(myHeap, hdVector_new, j, writeData);
			writeData = origData;
		}
		vector_write_data(myHeap, hdVector_new, currentVecSize, writeData);		
		vector_write_size(myHeap, hdVector_new, currentVecSize + 1);		
		
		SysFree(hdVector,Master2SysAlloc);
		return hdVector_new;
	}	
}

void VectorDelete(data_t *Master2Mem, ptr_t hdVector, int keyData){
	data_t *myHeap = Master2Mem;
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

ptr_t VectorDestroy(data_t *Master2SysAlloc, ptr_t hdVector){
	SysFree(hdVector, Master2SysAlloc);
	return NULL_PTR;
}

/* memory access functions */
int vector_read_volumn(data_t *Master2Mem, ptr_t hdVector){
	data_t *myHeap = Master2Mem;
	return myHeap[hdVector/4 + VECTOR_VOLUMN];
}
int vector_read_size(data_t *Master2Mem, ptr_t hdVector){
	data_t *myHeap = Master2Mem;
	return myHeap[hdVector/4 + VECTOR_SIZE];	
}
int vector_read_data(data_t *Master2Mem, ptr_t hdVector, int index){
	data_t *myHeap = Master2Mem;
	return myHeap[hdVector/4 + VECTOR_CONTENT + index];
}

void vector_write_volumn(data_t *Master2Mem, ptr_t hdVector, int volumn){
	data_t *myHeap = Master2Mem;
	myHeap[hdVector/4 + VECTOR_VOLUMN] = volumn;
}
void vector_write_size(data_t *Master2Mem, ptr_t hdVector, int index){
	data_t *myHeap = Master2Mem;
	myHeap[hdVector/4 + VECTOR_SIZE] = index;	
}
void vector_write_data(data_t *Master2Mem, ptr_t hdVector, int index, int data){
	data_t *myHeap = Master2Mem;
	myHeap[hdVector/4 + VECTOR_CONTENT + index] = data;
}
