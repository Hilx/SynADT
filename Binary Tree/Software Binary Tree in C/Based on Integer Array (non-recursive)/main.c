#include "main.h"

int main(void){
	/* Heap, Stack */
	myHeapIndex = 0;
	int *myHeap = (int*)malloc(sizeof(int)*NODE_SIZE*HEAP_SIZE); 
	int *stackPtr = NULL;
	ptr_t root;
	ptr_t returnedPtr;
	struct insert_t subResult;
	
	/* Application Code */
	printf("myHeap = %d\n",myHeap);
	root = TreeGen(myHeap, stackPtr, 5);
	printf("root = %d\n",root);
	
	return 0;	
}

