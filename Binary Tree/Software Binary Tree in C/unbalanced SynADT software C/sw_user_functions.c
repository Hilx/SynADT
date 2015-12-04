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