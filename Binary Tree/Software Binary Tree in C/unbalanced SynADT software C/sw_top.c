#include "sw_top.h"

int main(){
	int *root = NULL;
	
	/* Create the tree */
	int NumberOfNodes = 10; 
	// look into TreeGen function! Now it uses a fixed-size array of inputs
	root = TreeGen(NumberOfNodes, root); 	
	printTree(root);
	
	/* Check then Insert */
	int *temp = Check_thenInsert(root, 9);
	printTree(root);
	temp = Check_thenInsert(root, 4);
	printTree(root);
		
	/* Search */	
	UpdateNode(root, 2,14);
	printTree(root);
	
	/* Delete the three */
	root = DeleteTree(root);	
	printTree(root);
}