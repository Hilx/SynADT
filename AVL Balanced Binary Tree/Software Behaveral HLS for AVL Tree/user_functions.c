#include "top.h"

/* software only */
void printTree(int *myHeap, ptr_t nowPtr){
	printf("Printing the tree now [ ");
	printValue(myHeap, nowPtr);
	printf(" ] Done printing.\n");
}
void printValue(int *myHeap, ptr_t nowPtr){
	if(nowPtr != NULL_PTR){
		printValue(myHeap, node_get_left_pointer(myHeap, nowPtr));
		printf("%d",node_read_data(myHeap, nowPtr));
		printf(" ");
		printValue(myHeap, node_get_right_pointer(myHeap, nowPtr));
	}
}

/* Performance Measurements */
ptr_t PM_1_INSERTION(int *myHeap, int *fixedStack, int stackPtr_avl, int log2_tree_size){
	ptr_t root = NULL_PTR;
	if(TestCase == USE_SEQUENTIAL_KEYS){
		int m = log2_tree_size - 1;
		int iter = 1 << m;
		int i;		
		for(i = 0; i < iter; i++){
			int key = 16*(i+1);
			struct ptrBundle_t temp = Insert(myHeap, fixedStack, stackPtr_avl, root, key, NULL_PTR, NULL_PTR);
			root = temp.root;
		}
	}else{	
		int log2_RNG_num_range = log2_tree_size - 1; // generate half the range from RNG
		int IterationRange = 1 << log2_RNG_num_range;
		int i;
		for(i = 0; i < IterationRange; i++){

			int key = RandGen(log2_RNG_num_range);

			struct ptrBundle_t temp = Insert(myHeap, fixedStack, stackPtr_avl, root, key, NULL_PTR, NULL_PTR);
			root = temp.root;

		}
	}
	return root;	
}

ptr_t PM_2_CHECK_INSERTION(int *myHeap, int *fixedStack, int stackPtr_avl, ptr_t root, int log2_tree_size){
	if(TestCase == USE_SEQUENTIAL_KEYS){
		int m = log2_tree_size - 1;
		int iter = 1 << m;
		int i;
		for(i = 0; i < iter; i++){
			int key = RandGen(log2_tree_size+2);
			root = Check_thenInsert(myHeap, fixedStack, stackPtr_avl, root, key);
		}
	}else{	
		int log2_RNG_num_range = log2_tree_size; // generate FULL range from RNG
		int IterationRange = 1 << (log2_RNG_num_range - 1); // only get half of them
		int i;
		for(i = 0; i < IterationRange; i++){		
			int key = RandGen(log2_RNG_num_range);
			root = Check_thenInsert(myHeap, fixedStack, stackPtr_avl, root, key);
		}
	}
	return root;	
}

ptr_t PM_3_UPDATE(int *myHeap, int *fixedStack, int *secondStack, int stackPtr_avl, ptr_t root, int log2_tree_size){

	if(TestCase == USE_SEQUENTIAL_KEYS){
		int m = log2_tree_size - 1;
		int iter = 1 << m;
		int i;
		for(i = 0; i < iter; i++){
			int oldKey = RandGen(log2_tree_size+2);
			int newKey = RandGen(log2_tree_size+3);
			root = UpdateNode(myHeap, fixedStack, secondStack, stackPtr_avl, root, oldKey, newKey);
		}
	}else{
		int IterationRange = 1 << (log2_tree_size - 1);
		int i;
		for(i = 0; i < IterationRange; i++){
			int oldKey = RandGen(log2_tree_size - 1);
			int newKey = RandGen(log2_tree_size);
			root = UpdateNode(myHeap, fixedStack, secondStack, stackPtr_avl, root, oldKey, newKey);
		}
	}
	return root;
}

ptr_t PM_4_DELETION(int *myHeap, int *fixedStack, int stackPtr_avl, ptr_t root){
	return DeleteTree(myHeap, fixedStack, stackPtr_avl, root);
}
