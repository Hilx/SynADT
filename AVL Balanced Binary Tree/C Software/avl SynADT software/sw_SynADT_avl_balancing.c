#include "sw_SynADT.h"

int max(int a, int b){
	if(a > b){
		return a;
	}else{
		return b;
	}
}

int calc_height(int *nodePtr){
	int *leftPtr = tree_get_left_pointer(nodePtr);
	int *rightPtr = tree_get_right_pointer(nodePtr);
	int height, leftHeight, rightHeight;
	
	if(nodePtr != NULL){
		if(leftPtr == NULL && rightPtr == NULL){
			height = 0;
		}else{
			if(leftPtr != NULL){
				leftHeight = tree_node_read_height(leftPtr);
			}else{
				leftHeight = 0;
			}
			
			if(rightPtr != NULL){
				rightHeight = tree_node_read_height(rightPtr);
			}else{
				rightHeight = 0;
			}
			
			height = max(leftHeight, rightHeight) + 1;			
		}
	}else{
		height = 0; // never calcutlate the height of a NULL pointer, if wonders, return 0
	}
	return height;	 
	
}

int tree_get_height(int *tree){
	if(tree == NULL){
		return 0;
	}else{
		return tree_node_read_height(tree);
	}
}

int *rightRotate(int *y){
    int *x  = tree_get_left_pointer(y);
    int *T2 = tree_get_right_pointer(x);
 
    // Perform rotation
    tree_set_right_pointer(x,y);
    tree_set_left_pointer(y,T2);
 
    // Update heights
	tree_node_write_height(y, calc_height(y));
	tree_node_write_height(x, calc_height(x));
 
    // Return new root
    return x;
}
 
int *leftRotate(int *x){
    int *y = tree_get_right_pointer(x);
    int *T2 = tree_get_left_pointer(y);
 
    // Perform rotation
	tree_set_left_pointer(y,x);
	tree_set_right_pointer(x,T2);
 
    //  Update heights
	tree_node_write_height(x, calc_height(x));
	tree_node_write_height(y, calc_height(y));	
 
    // Return new root
    return y;
}



signed getBalance(int *nodePtr){
	int *leftPtr = tree_get_left_pointer(nodePtr);
	int *rightPtr = tree_get_right_pointer(nodePtr);

	if(nodePtr == NULL){
		return 0;
	}else{
		if(leftPtr == NULL && rightPtr == NULL){
			return 0;
		}else{
		int heightLeft = tree_get_height(leftPtr);
		int heightRight = tree_get_height(rightPtr);
		return heightLeft - heightRight;
		}
	}
}