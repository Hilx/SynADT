#include "main.h"

int main(void){
	/* Heap, Stack */
	myHeapIndex = 0;
	int *myHeap = (int*)malloc(sizeof(int)*NODE_SIZE*HEAP_SIZE); 
	int *stackPtr = NULL;
	ptr_t root;
	struct search_t searchResult;
	struct sub_t subResult;
	int searchKey = 3;
	
	/* Application Code */
	/* Create Tree */
	root = TreeGen(myHeap, stackPtr, 7);
	printTree(myHeap, root);


	/* Update node */
	root = UpdateNode(myHeap, stackPtr, root, 3,10);
	printTree(myHeap, root);

	/* Delete Tree */
	root = DeleteTree(myHeap, stackPtr, root);
	printTree(myHeap,root);
	
	return 0;	
}

