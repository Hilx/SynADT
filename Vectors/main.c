#include "main.h"

int main(){
	int *myHeap = (int*)malloc(sizeof(int) * HEAP_SIZE);
	ptr_t hdVector;
	
	hdVector = VectorCreate(myHeap);
	printf("hdVector = %d\n", hdVector); 
	PrintVec(myHeap, hdVector);
	
	hdVector = VectorInit(myHeap, hdVector, 4);
	printf("hdVector = %d\n", hdVector); 
	PrintVec(myHeap, hdVector);
	
	hdVector = VectorDestroy(myHeap, hdVector);
	printf("hdVector = %d\n", hdVector); 
	PrintVec(myHeap, hdVector);
			
	return 0;
}
