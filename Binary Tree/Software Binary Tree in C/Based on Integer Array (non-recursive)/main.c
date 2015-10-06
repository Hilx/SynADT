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
	
	//printTree(myHeap, root);
	int i;
	for(i = 0; i<5; i++){
		printf("%d,%d,%d\n",myHeap[i*3+2],myHeap[i*3+0], myHeap[i*3+1]);
	}
	
	printTree(myHeap, root);	
	return 0;	
}

