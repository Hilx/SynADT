#include "sw_top.h"

int main(){
	int *root = NULL;
	int log2_tree_size = 4;
	
	root = PM_1_INSERTION(root, log2_tree_size);
	printTree(root);
	root = PM_2_CHECK_INSERTION(root, log2_tree_size);
	printTree(root);
	
	root = PM_3_UPDATE(root, log2_tree_size);
	printTree(root);

	root = PM_4_DELETION(root);
	printTree(root);
}