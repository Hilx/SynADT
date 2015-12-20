#include "top.h"

int main(void){
	/* Heap, Stack */
	int *fixedStack = (int*)malloc(sizeof(int)*33);
	int stackPtr_avl = 0;	
	int *myHeap = (int*)malloc(sizeof(int)*NODE_SIZE*HEAP_SIZE); 

	ptr_t root;
	
	/* Application Code */
	/* Create Tree */
	//root = TreeGen(myHeap, fixedStack, stackPtr_avl, 9);
	//printTree(myHeap, root);

	/* Update node */	
	//root = Check_thenInsert(myHeap, fixedStack, stackPtr_avl, root, 4);
	//printTree(myHeap, root);	
	/* Update node */	
	//root = Check_thenInsert(myHeap, fixedStack, stackPtr_avl, root, 1);
	//printTree(myHeap, root);	
	
	/* Update node */	
	//root = UpdateNode(myHeap, fixedStack, stackPtr_avl, root, 18,19);
	//printTree(myHeap, root);

	/* Delete Tree */
	//root = DeleteTree(myHeap, fixedStack, stackPtr_avl, root);
	//printTree(myHeap,root);
	
	int log2_tree_size = 7;
	
	root = PM_1_INSERTION(myHeap, fixedStack, stackPtr_avl, log2_tree_size);;
	printTree(myHeap, root);
	
	root = PM_2_CHECK_INSERTION(myHeap, fixedStack, stackPtr_avl, root, log2_tree_size);
	printTree(myHeap, root);
	

	root = PM_3_UPDATE(myHeap, fixedStack, stackPtr_avl, root, log2_tree_size);
	printTree(myHeap, root);

	root = PM_4_DELETION(myHeap, fixedStack, stackPtr_avl, root);
	printTree(myHeap, root);
	
	
	
	return 0;	
}

