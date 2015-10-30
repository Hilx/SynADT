#include "main.h"

int main(void){
	/* Heap, Stack */
	int *fixedStack = (int*)malloc(sizeof(int)*33);
	int stackPtr_avl = 0;	
	int *myHeap = (int*)malloc(sizeof(int)*NODE_SIZE*HEAP_SIZE); 

	ptr_t root;
	
	/* Application Code */
	/* Create Tree */
	root = TreeGen(myHeap, fixedStack, stackPtr_avl, 9);
	printTree(myHeap, root);

	/* Update node */	
	root = UpdateNode(myHeap, fixedStack, stackPtr_avl, root, 7,10);
	printTree(myHeap, root);

	/* Delete Tree */
	root = DeleteTree(myHeap, fixedStack, stackPtr_avl, root);
	printTree(myHeap,root);
	
	return 0;	
}

