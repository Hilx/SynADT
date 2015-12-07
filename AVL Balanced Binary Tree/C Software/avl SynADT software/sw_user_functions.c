/* For testing purposes */
#include "sw_top.h"

/* Performance Measurements */
int *PM_1_INSERTION(int *root, int log2_tree_size){
	int log2_RNG_num_range = log2_tree_size - 1; // generate half the range from RNG
	int IterationRange = 1 << log2_RNG_num_range;
	int *temp;
	int i;
	for(i = 0; i < IterationRange; i++){
		int key = RandGen(log2_RNG_num_range);
		root = Insert(key,root);
	}
	return root;	
}

int *PM_2_CHECK_INSERTION(int *root, int log2_tree_size){
	int log2_RNG_num_range = log2_tree_size; // generate FULL range from RNG
	int IterationRange = 1 << (log2_RNG_num_range - 1); // only get half of them
	int i;
	for(i = 0; i < IterationRange; i++){		
		int key = RandGen(log2_RNG_num_range);
		root = Check_thenInsert(root,key);
	}
	return root;	
}

int *PM_3_UPDATE(int *root, int log2_tree_size){
	int IterationRange = 1 << (log2_tree_size - 1);
	int i;
	for(i = 0; i < IterationRange; i++){
		int oldKey = RandGen(log2_tree_size - 1);
		int newKey = RandGen(log2_tree_size); 
		printf("%d %d\n",oldKey, newKey);
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

