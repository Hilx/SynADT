#include "SynADT_header.h"

int max(int a, int b){
	if( a > b){
		return a;
	}else{
		return b;
	}
}

int CalculateNodeHeight(data_t *Master2Mem, ptr_t nodePtr){	
	data_t *myHeap = Master2Mem;
	ptr_t leftPtr = tree_node_get_left_pointer(myHeap, nodePtr);
	ptr_t rightPtr = tree_node_get_right_pointer(myHeap, nodePtr);
	int height, leftHeight, rightHeight;
	
	if(nodePtr != NULL_PTR){
		if(leftPtr == NULL_PTR && rightPtr == NULL_PTR){
			height = 0;
		}else{
			if(leftPtr != NULL_PTR){
				leftHeight = tree_node_read_height(myHeap, leftPtr);
			}else{
				leftHeight = 0;
			}
			
			if(rightPtr != NULL_PTR){
				rightHeight = tree_node_read_height(myHeap, rightPtr);
			}else{
				rightHeight = 0;
			}
			
			height = max(leftHeight, rightHeight) + 1;			
		}
	}else{
		height = 1000;
	}
	return height;
}

int height(data_t *Master2Mem, ptr_t nodePtr){
	data_t *myHeap = Master2Mem;
	if(nodePtr == NULL_PTR){
		return 0;
	}else{
		return tree_node_read_height(myHeap, nodePtr);
	}
}

ptr_t RightRotate(data_t *Master2Mem, ptr_t yPtr){
	data_t *myHeap = Master2Mem;
	ptr_t xPtr = tree_node_get_left_pointer(myHeap, yPtr);
	ptr_t temp = tree_node_get_right_pointer(myHeap, xPtr);

	tree_node_set_right(myHeap, xPtr, yPtr);
	tree_node_set_left(myHeap, yPtr, temp);
	
	tree_node_set_height(myHeap, yPtr, CalculateNodeHeight(myHeap, yPtr));
	tree_node_set_height(myHeap, xPtr, CalculateNodeHeight(myHeap, xPtr));
	
	return xPtr;	
}

ptr_t LeftRotate(data_t *Master2Mem, ptr_t xPtr){
	data_t *myHeap = Master2Mem;
	ptr_t yPtr = tree_node_get_right_pointer(myHeap, xPtr);
	ptr_t temp = tree_node_get_left_pointer(myHeap, yPtr);
	
	tree_node_set_left(myHeap, yPtr, xPtr);
	tree_node_set_right(myHeap, xPtr, temp);
	
	tree_node_set_height(myHeap, xPtr, CalculateNodeHeight(myHeap, xPtr));
	tree_node_set_height(myHeap, yPtr, CalculateNodeHeight(myHeap, yPtr));
	
	return yPtr;
}

signed GetBalance(data_t *Master2Mem, ptr_t nodePtr){
	data_t *myHeap = Master2Mem;	
	if(nodePtr == NULL_PTR){
		return 0;
	}else{
		return height(myHeap, tree_node_get_left_pointer(myHeap, nodePtr)) - height(myHeap, tree_node_get_right_pointer(myHeap, nodePtr));
	}
}

ptr_t ProcessNodeInsertion(data_t *Master2Mem, ptr_t nowPtr, int key){
	data_t *myHeap = Master2Mem;	
	int newHeight;
	signed balanceFactor;
	int nowKey, leftKey, rightKey;
	ptr_t leftPtr, rightPtr, outputPtr, tempPtr;
	
	// update height of this ancestor node
	leftPtr = tree_node_get_left_pointer(myHeap, nowPtr);
	rightPtr = tree_node_get_right_pointer(myHeap, nowPtr);
	newHeight = CalculateNodeHeight(myHeap, nowPtr);
	tree_node_set_height(myHeap, nowPtr, newHeight);
	
	// get balance factor for this ancestor
	balanceFactor = GetBalance(myHeap, nowPtr);
	// if unbalanced, there are 4 cases
	leftKey = tree_node_read_data(myHeap, leftPtr);
	rightKey = tree_node_read_data(myHeap, rightPtr);

	if(balanceFactor > 1 &&  key < leftKey){
		outputPtr = RightRotate(myHeap, nowPtr);
	}else if(balanceFactor < -1 && key > rightKey){
		outputPtr = LeftRotate(myHeap, nowPtr);
	}else if(balanceFactor > 1 && key > leftKey){
		tree_node_set_left(myHeap, nowPtr, LeftRotate(myHeap, leftPtr));
		outputPtr = RightRotate(myHeap,nowPtr);		
	}else if(balanceFactor < -1 && key < rightKey){
		tree_node_set_right(myHeap, nowPtr, RightRotate(myHeap, rightPtr));
		outputPtr = LeftRotate(myHeap,nowPtr);			
	}else{
		outputPtr = nowPtr;
	}	
	return outputPtr;
}

ptr_t ProcessNodeDeletion(data_t *Master2Mem, ptr_t nowPtr){
	data_t *myHeap = Master2Mem;	
	ptr_t outputPtr;
	int newHeight;
	signed balanceFactor;
	ptr_t leftPtr, rightPtr;	
	
	if(nowPtr == NULL_PTR){
		outputPtr = nowPtr;
	}else{
		// update height
		newHeight = CalculateNodeHeight(myHeap, nowPtr);
		tree_node_set_height(myHeap, nowPtr, newHeight);
		
		//get balance factor
		balanceFactor = GetBalance(myHeap, nowPtr);
		
		// if unbalanced, there are 4 cases
		leftPtr = tree_node_get_left_pointer(myHeap, nowPtr);
		rightPtr = tree_node_get_right_pointer(myHeap, nowPtr);
		if(balanceFactor > 1 && GetBalance(myHeap, leftPtr) >= 0){
			outputPtr = RightRotate(myHeap, nowPtr);
		}else if(balanceFactor > 1 && GetBalance(myHeap, leftPtr) <0 ){
			tree_node_set_left(myHeap, nowPtr, LeftRotate(myHeap, leftPtr));
			outputPtr = RightRotate(myHeap,nowPtr);					
		}else if(balanceFactor < -1 && GetBalance(myHeap, rightPtr) <= 0){
			outputPtr = LeftRotate(myHeap, nowPtr);
		}else if(balanceFactor < -1 && GetBalance(myHeap, rightPtr) > 0){
			tree_node_set_right(myHeap, nowPtr, RightRotate(myHeap, rightPtr));
			outputPtr = LeftRotate(myHeap,nowPtr);				
		}else{
			outputPtr = nowPtr;
		}	
	}	
	return outputPtr;
}