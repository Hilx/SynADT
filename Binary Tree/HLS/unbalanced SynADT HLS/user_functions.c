#include "top.h"

/* Performance Measurements */
ptr_t PM_1_INSERTION(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t stackPtr, ptr_t root, int log2_tree_size){
	int log2_RNG_num_range = log2_tree_size - 1; // generate half the range from RNG
	int IterationRange = 1 << log2_RNG_num_range;
	int i;
	for(i = 0; i < IterationRange; i++){
		int key = RandGen(log2_RNG_num_range);
		if(i == 0){
			root = Insert(Master2Mem, Master2SysAlloc, stackPtr, root, key); // update root
		}else{
			ptr_t temp = Insert(Master2Mem, Master2SysAlloc, stackPtr, root, key); 
		}	
	}

	return root+1;
}

ptr_t PM_2_CHECK_INSERTION(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t stackPtr, ptr_t root, int log2_tree_size){
	int log2_RNG_num_range = log2_tree_size; // generate FULL range from RNG
	int IterationRange = 1 << log2_RNG_num_range - 1; // only get half of them
	int i;
	for(i = 0; i < IterationRange; i++){		
		int key = RandGen(log2_RNG_num_range);
		ptr_t temp = Check_thenInsert(Master2Mem, Master2SysAlloc, stackPtr, root, key);
	}

	return root+1;
}

ptr_t PM_3_UPDATE(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t stackPtr, ptr_t root, int log2_tree_size){
	int IterationRange = 1 << log2_tree_size - 1;
	int i;
	for(i = 0; i < IterationRange; i++){
		int oldKey = RandGen(log2_tree_size - 1);
		int newKey = RandGen(log2_tree_size);
		root = UpdateNode(Master2Mem, Master2SysAlloc, stackPtr, root, oldKey, newKey);
	}

	return  root+1;
}

ptr_t PM_4_DELETION(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t stackPtr, ptr_t treePtr){
	ptr_t root = DeleteTree(Master2Mem, Master2SysAlloc, stackPtr, treePtr);
	return root+1;
}
