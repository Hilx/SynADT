#include "top.h"

/* software only */
void printTree(int *myHeap, ptr_t nowPtr){
	printf("Printing the tree now [ ");
	printValue(myHeap, nowPtr);
	printf(" ] Done printing.\n");
}
void printValue(int *myHeap, ptr_t nowPtr){
	if(nowPtr != NULL_PTR){
		printValue(myHeap, node_get_leftNodePtr(myHeap, nowPtr));
		printf("%d ", node_read_data(myHeap, nowPtr));
		printValue(myHeap, node_get_rightNodePtr(myHeap, nowPtr));
	}
}

/* Performance Measurements */
ptr_t PM_1_INSERTION(int *myHeap, int *fixedStack, int stackPtr_avl, int log2_tree_size){
	int log2_RNG_num_range = log2_tree_size - 1; // generate half the range from RNG
	int IterationRange = 1 << log2_RNG_num_range;
	ptr_t root = NULL_PTR;
	int i;
	for(i = 0; i < IterationRange; i++){
		int key = RandGen(log2_RNG_num_range);
		struct ptrBundle_t temp = Insert(myHeap, fixedStack, stackPtr_avl, root, key, NULL_PTR, NULL_PTR);
		root = temp.root;
	}
	return root;	
}

ptr_t PM_2_CHECK_INSERTION(int *myHeap, int *fixedStack, int stackPtr_avl, ptr_t root, int log2_tree_size){
	int log2_RNG_num_range = log2_tree_size; // generate FULL range from RNG
	int IterationRange = 1 << (log2_RNG_num_range - 1); // only get half of them
	int i;
	for(i = 0; i < IterationRange; i++){		
		int key = RandGen(log2_RNG_num_range);
		root = Check_thenInsert(myHeap, fixedStack, stackPtr_avl, root, key);
	}
	return root;	
}

ptr_t PM_3_UPDATE(int *myHeap, int *fixedStack, int stackPtr_avl, ptr_t root, int log2_tree_size){
	int IterationRange = 1 << (log2_tree_size - 1);
	int i;
	for(i = 0; i < IterationRange; i++){
		
		int oldKey = RandGen(log2_tree_size - 1);
		int newKey = RandGen(log2_tree_size);
		root = UpdateNode(myHeap, fixedStack, stackPtr_avl, root, oldKey, newKey);
	}
	return root;
}

ptr_t PM_4_DELETION(int *myHeap, int *fixedStack, int stackPtr_avl, ptr_t root){
	return DeleteTree(myHeap, fixedStack, stackPtr_avl, root);
}