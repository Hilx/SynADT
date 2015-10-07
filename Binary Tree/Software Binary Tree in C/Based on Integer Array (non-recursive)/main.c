#include "main.h"

int main(void){
	/* Heap, Stack */
	myHeapIndex = 0;
	int *myHeap = (int*)malloc(sizeof(int)*NODE_SIZE*HEAP_SIZE); 
	int *stackPtr = NULL;
	ptr_t root;
	ptr_t returnedPtr;
	struct sub_t subResult;
	int searchKey = 3;
	
	/* Application Code */
	/* Create Tree */
	root = TreeGen(myHeap, stackPtr, 7);
	printTree(myHeap, root);	
	/*Search */
	returnedPtr = Search(myHeap, stackPtr, root, searchKey);
	printf("searchKey %d found at %d, myHeap[%d] has data %d\n", searchKey, returnedPtr,returnedPtr, myHeap[returnedPtr+DATA_OFFSET]);
	/* Delete Tree*/
	root = DeleteTree(myHeap, stackPtr, root);
	printTree(myHeap, root);
	
	return 0;	
}

