/* For testing purposes */
#include "sw_top.h"

int *TreeGen(int NumberOfNodes, int *root){
	int i;
	int *temp;
	int array[10] = {5,4,8,2,1,4,6,7,11,0}; // can be randomly generated
	NumberOfNodes = 10;
	for(i = 0; i <NumberOfNodes; i++){
		if(i == 0){
			root = Insert(array[i], root);
		}else{
			temp = Insert(array[i], root);
		}
	}
	return root;	
}

/* Performance Measurements */
int *PM_1_INSERTION(int *root, int log2_tree_size){
	int log2_RNG_num_range = log2_tree_size - 1; // generate half the range from RNG
	int IterationRange = 1 << log2_RNG_num_range;
	int *temp;
	int i;
	for(i = 0; i < IterationRange; i++){
		//putnum(i);
		//print("\n");
		int key = RandGen(log2_RNG_num_range);
		if(i == 0){
			root = Insert(key,root); // update root
		}else{
			temp = Insert(key,root);
		}	
	}
	//print("done pm1\n");
	return root;	
}

int *PM_2_CHECK_INSERTION(int *root, int log2_tree_size){
	int log2_RNG_num_range = log2_tree_size; // generate FULL range from RNG
	int IterationRange = 1 << (log2_RNG_num_range - 1); // only get half of them
	int *temp;
	int i;
	for(i = 0; i < IterationRange; i++){		
		int key = RandGen(log2_RNG_num_range);
		temp = Check_thenInsert(root,key);
	}
	return root;	
}

int *PM_3_UPDATE(int *root, int log2_tree_size){
	int IterationRange = 1 << (log2_tree_size - 1);
	int i;
	for(i = 0; i < IterationRange; i++){
		int oldKey = RandGen(log2_tree_size - 1);
		int newKey = RandGen(log2_tree_size);
		root = UpdateNode(root, oldKey, newKey);
	}
	return root;
}

int *PM_4_DELETION(int *root){
	return DeleteTree(root);
}



/* software only */
void printTree(int *tree){
	printf("Printing the tree now [ ");
	printValue(tree);
	printf(" ] Done printing.\n");
}
void printValue(int *tree){
	if(tree != NULL){
		printValue(tree_get_left_pointer(tree));
		printf(" %d ",tree_node_read_data(tree));
		printValue(tree_get_right_pointer(tree));
	}
}

