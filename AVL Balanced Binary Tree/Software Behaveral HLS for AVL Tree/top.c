#include "top.h"

int main(void){
	/* Heap, Stack */
	int *fixedStack = (int*)malloc(sizeof(int)*33);
	int *secondStack = (int*)malloc(sizeof(int)*33);
	int stackPtr_avl = 0;	
	int *myHeap = (int*)malloc(sizeof(int)*NODE_SIZE*HEAP_SIZE); 

	ptr_t root;

	int i;
	TestCase = USE_RANDOM_KEYS;

	int log2_tree_size = 10;
		
		root = PM_1_INSERTION(myHeap, fixedStack, stackPtr_avl, log2_tree_size);;
		//printTree(myHeap, root);
		
		root = PM_2_CHECK_INSERTION(myHeap, fixedStack, stackPtr_avl, root, log2_tree_size);
		//printTree(myHeap, root);

		root = PM_3_UPDATE(myHeap, fixedStack, secondStack, stackPtr_avl, root, log2_tree_size);
		//printTree(myHeap, root);

		root = PM_4_DELETION(myHeap, fixedStack, stackPtr_avl, root);
		//printTree(myHeap, root);
	printf("done\n");
	return 0;	
}

