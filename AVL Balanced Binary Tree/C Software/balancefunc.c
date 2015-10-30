#include "main.h"

int max(int a, int b){
	if( a > b){
		return a;
	}else{
		return b;
	}
}

int CalculateNodeHeight(int *myHeap, ptr_t nodePtr){	
	ptr_t leftPtr = node_get_leftNodePtr(myHeap, nodePtr);
	ptr_t rightPtr = node_get_rightNodePtr(myHeap, nodePtr);
	int height, leftHeight, rightHeight;
	
	if(nodePtr != NULL_PTR){
		if(leftPtr == NULL_PTR && rightPtr == NULL_PTR){
			height = 0;
		}else{
			if(leftPtr != NULL_PTR){
				leftHeight = node_read_height(myHeap, leftPtr);
			}else{
				leftHeight = 0;
			}
			
			if(rightPtr != NULL_PTR){
				rightHeight = node_read_height(myHeap, rightPtr);
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

int height(int *myHeap, ptr_t nodePtr){
	if(nodePtr == NULL_PTR){
		return 0;
	}else{
		return node_read_height(myHeap, nodePtr);
	}
}

ptr_t RightRotate(int *myHeap, ptr_t yPtr){
	ptr_t xPtr = node_get_leftNodePtr(myHeap, yPtr);
	ptr_t temp = node_get_rightNodePtr(myHeap, xPtr);

	node_set_right(myHeap, xPtr, yPtr);
	node_set_left(myHeap, yPtr, temp);
	
	node_set_height(myHeap, yPtr, CalculateNodeHeight(myHeap, yPtr));
	node_set_height(myHeap, xPtr, CalculateNodeHeight(myHeap, xPtr));
	
	return xPtr;	
}

ptr_t LeftRotate(int *myHeap, ptr_t xPtr){
	ptr_t yPtr = node_get_rightNodePtr(myHeap, xPtr);
	ptr_t temp = node_get_leftNodePtr(myHeap, yPtr);
	
	node_set_left(myHeap, yPtr, xPtr);
	node_set_right(myHeap, xPtr, temp);
	
	node_set_height(myHeap, xPtr, CalculateNodeHeight(myHeap, xPtr));
	node_set_height(myHeap, yPtr, CalculateNodeHeight(myHeap, yPtr));
	
	return yPtr;
}

signed GetBalance(int *myHeap, ptr_t nodePtr){
	if(nodePtr == NULL_PTR){
		return 0;
	}else{
		return height(myHeap, node_get_leftNodePtr(myHeap, nodePtr)) - height(myHeap, node_get_rightNodePtr(myHeap, nodePtr));
	}
}

ptr_t ProcessNodeInsertion(int *myHeap, ptr_t nowPtr, int key){
	int newHeight;
	signed balanceFactor;
	int nowKey, leftKey, rightKey;
	ptr_t leftPtr, rightPtr, outputPtr, tempPtr;
	
	// update height of this ancestor node
	leftPtr = node_get_leftNodePtr(myHeap, nowPtr);
	rightPtr = node_get_rightNodePtr(myHeap, nowPtr);
	newHeight = CalculateNodeHeight(myHeap, nowPtr);
	node_set_height(myHeap, nowPtr, newHeight);
	
	// get balance factor for this ancestor
	balanceFactor = GetBalance(myHeap, nowPtr);
	// if unbalanced, there are 4 cases
	leftKey = node_read_data(myHeap, leftPtr);
	rightKey = node_read_data(myHeap, rightPtr);

	if(balanceFactor > 1 &&  key < leftKey){
		outputPtr = RightRotate(myHeap, nowPtr);
	}else if(balanceFactor < -1 && key > rightKey){
		outputPtr = LeftRotate(myHeap, nowPtr);
	}else if(balanceFactor > 1 && key > leftKey){
		node_set_left(myHeap, nowPtr, LeftRotate(myHeap, leftPtr));
		outputPtr = RightRotate(myHeap,nowPtr);		
	}else if(balanceFactor < -1 && key < rightKey){
		node_set_right(myHeap, nowPtr, RightRotate(myHeap, rightPtr));
		outputPtr = LeftRotate(myHeap,nowPtr);			
	}else{
		outputPtr = nowPtr;
	}
	
	return outputPtr;
}

ptr_t ProcessNodeDeletion(int *myHeap, ptr_t nowPtr){
	ptr_t outputPtr;
	int newHeight;
	signed balanceFactor;
	ptr_t leftPtr, rightPtr;	
	
	if(nowPtr == NULL_PTR){
		outputPtr = nowPtr;
	}else{
		// update height
		newHeight = CalculateNodeHeight(myHeap, nowPtr);
		node_set_height(myHeap, nowPtr, newHeight);
		
		//get balance factor
		balanceFactor = GetBalance(myHeap, nowPtr);
		
		// if unbalanced, there are 4 cases
		leftPtr = node_get_leftNodePtr(myHeap, nowPtr);
		rightPtr = node_get_rightNodePtr(myHeap, nowPtr);
		if(balanceFactor > 1 && GetBalance(myHeap, leftPtr) >= 0){
			outputPtr = RightRotate(myHeap, nowPtr);
		}else if(balanceFactor > 1 && GetBalance(myHeap, leftPtr) <0 ){
			node_set_left(myHeap, nowPtr, LeftRotate(myHeap, leftPtr));
			outputPtr = RightRotate(myHeap,nowPtr);					
		}else if(balanceFactor < -1 && GetBalance(myHeap, rightPtr) <= 0){
			outputPtr = LeftRotate(myHeap, nowPtr);
		}else if(balanceFactor < -1 && GetBalance(myHeap, rightPtr) > 0){
			node_set_right(myHeap, nowPtr, RightRotate(myHeap, rightPtr));
			outputPtr = LeftRotate(myHeap,nowPtr);				
		}else{
			outputPtr = nowPtr;
		}	
	}	
	return outputPtr;
}