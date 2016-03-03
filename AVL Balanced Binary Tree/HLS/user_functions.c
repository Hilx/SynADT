#include "top.h"

/* Performance Measurements */
ptr_t PM_1_INSERTION(data_t *myHeap, data_t *Master2SysAlloc, int *fixedStack, int stackPtr_avl, int log2_tree_size){
	ptr_t root = NULL_PTR;
	if(TestCase == USE_RANDOM_KEYS){
		int log2_RNG_num_range = log2_tree_size - 1; // generate half the range from RNG
		int IterationRange = 1 << log2_RNG_num_range;
		int i;
		for(i = 0; i < IterationRange; i++){

			int key = RandGen(log2_RNG_num_range);

			struct ptrBundle_t temp = Insert(myHeap, Master2SysAlloc, fixedStack, stackPtr_avl, root, key);
			root = temp.root;

		}
	}else{
		int m = log2_tree_size - 1;
		int iter = 1 << m;
		int i;
		for(i = 0; i < iter; i++){
			int key = 16*(i+1);
			struct ptrBundle_t temp = Insert(myHeap, Master2SysAlloc, fixedStack, stackPtr_avl, root, key);
			root = temp.root;
		}
	}

	root = root +1;
	return root;
}

ptr_t PM_2_CHECK_INSERTION(data_t *myHeap, data_t *Master2SysAlloc, int *fixedStack, int stackPtr_avl, ptr_t root, int log2_tree_size){
	if(TestCase == USE_SEQUENTIAL_KEYS){
		int m = log2_tree_size - 1;
		int iter = 1 << m;
		int i;
		for(i = 0; i < iter; i++){
			int key = RandGen(log2_tree_size+2);
			root = Check_thenInsert(myHeap, Master2SysAlloc, fixedStack, stackPtr_avl, root, key);
		}
	}else{	
		int log2_RNG_num_range = log2_tree_size; // generate FULL range from RNG
		int IterationRange = 1 << (log2_RNG_num_range - 1); // only get half of them
		int i;
		for(i = 0; i < IterationRange; i++){		
			int key = RandGen(log2_RNG_num_range);
			root = Check_thenInsert(myHeap, Master2SysAlloc, fixedStack, stackPtr_avl, root, key);
		}
	}
	return root+1;
}

ptr_t PM_3_UPDATE(data_t *myHeap, data_t *Master2SysAlloc, int *fixedStack, int *secondStack, int stackPtr_avl, ptr_t root, int log2_tree_size){

	if(TestCase == USE_SEQUENTIAL_KEYS){
		int m = log2_tree_size - 1;
		int iter = 1 << m;
		int i;
		for(i = 0; i < iter; i++){
			int oldKey = RandGen(log2_tree_size+2);
			int newKey = RandGen(log2_tree_size+3);
			root = UpdateNode(myHeap, Master2SysAlloc, fixedStack, secondStack, stackPtr_avl, root, oldKey, newKey);
		}
	}else{
		int IterationRange = 1 << (log2_tree_size - 1);
		int i;
		for(i = 0; i < IterationRange; i++){
			int oldKey = RandGen(log2_tree_size - 1);
			int newKey = RandGen(log2_tree_size);
			root = UpdateNode(myHeap, Master2SysAlloc, fixedStack, secondStack, stackPtr_avl, root, oldKey, newKey);
		}
	}
	return root+1;
}

ptr_t PM_4_DELETION(data_t *myHeap, data_t *Master2SysAlloc, int *fixedStack, int stackPtr_avl, ptr_t root){
	root = DeleteTree(myHeap, Master2SysAlloc, fixedStack, stackPtr_avl, root);
	return root + 1;
}

ptr_t add_new(data_t *myHeap, data_t *Master2SysAlloc, int *fixedStack, int stackPtr, ptr_t root, int key){
	struct ptrBundle_t temp = Insert(myHeap, Master2SysAlloc, fixedStack, stackPtr, root, key);
	root = temp.root;

	return root;
}
