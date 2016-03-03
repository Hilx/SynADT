#include "SynADT.h"

int max(int a, int b){
	if( a > b){
		return a;
	}else{
		return b;
	}
}

int CalculateNodeHeight(data_t *myHeap, ptr_t nodePtr){	
	int height;	
	if(nodePtr == NULL_PTR){
		height = 0;
	}else{
		ptr_t leftPtr = node_get_left_pointer(myHeap, nodePtr);
		ptr_t rightPtr = node_get_right_pointer(myHeap, nodePtr);
		int leftHeight, rightHeight;		
		if(leftPtr == NULL_PTR && rightPtr == NULL_PTR){
			height = 1;
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
	}
	return height;
}

int height(data_t *myHeap, ptr_t nodePtr){
	if(nodePtr == NULL_PTR){
		return 0;
	}else{
		return node_read_height(myHeap, nodePtr);
	}
}

ptr_t RightRotate(data_t *myHeap, ptr_t yPtr){
	volatile ptr_t xPtr = 1;
	volatile ptr_t temp = 1;

	xPtr = node_get_left_pointer(myHeap, yPtr);
	temp = node_get_right_pointer(myHeap, xPtr);
	
	volatile int a = 1, b = 1;

	if(temp != 1){
		node_set_left(myHeap, yPtr, temp);
		b = 0;
	}


	if(xPtr != 1){
		node_set_right(myHeap, xPtr, yPtr);
		a = 0;
	}

	if(b == 0){
		node_set_height(myHeap, yPtr, CalculateNodeHeight(myHeap, yPtr));
	}


	if(a == 0){
		node_set_height(myHeap, xPtr, CalculateNodeHeight(myHeap, xPtr));
	}

	return xPtr;	
}

ptr_t LeftRotate(data_t *myHeap, ptr_t xPtr){
	volatile ptr_t yPtr = 1;
	volatile ptr_t temp = 1;

	yPtr = node_get_right_pointer(myHeap, xPtr);
	temp = node_get_left_pointer(myHeap, yPtr);

	volatile int a = 1, b = 1;
	
	if(temp != 1){
		node_set_right(myHeap, xPtr, temp);
		a = 0;
	}
	if(yPtr != 1){
		node_set_left(myHeap, yPtr, xPtr);
		b = 0;
	}

	if(a == 0){
	node_set_height(myHeap, xPtr, CalculateNodeHeight(myHeap, xPtr));
	}
	if(b == 0){
	node_set_height(myHeap, yPtr, CalculateNodeHeight(myHeap, yPtr));
	}
	
	return yPtr;
}

signed GetBalance(data_t *myHeap, ptr_t nodePtr){
	if(nodePtr == NULL_PTR){
		return 0;
	}else{		
		ptr_t leftPtr = node_get_left_pointer(myHeap, nodePtr);
		ptr_t rightPtr = node_get_right_pointer(myHeap, nodePtr);
		if(leftPtr == NULL_PTR && rightPtr == NULL_PTR){
			return 0;
		}else{
			int heightLeft = height(myHeap, leftPtr);
			int heightRight = height(myHeap, rightPtr);
			return heightLeft - heightRight;
		}
	}
}

ptr_t ProcessNodeInsertion(data_t *myHeap, ptr_t nowPtr, int key){
	int newHeight;
	signed balanceFactor;
	int nowKey, leftKey, rightKey;
	ptr_t leftPtr, rightPtr, outputPtr, tempPtr;
	
	// update height of this ancestor node
	leftPtr = node_get_left_pointer(myHeap, nowPtr);
	rightPtr = node_get_right_pointer(myHeap, nowPtr);

	leftKey = node_read_data(myHeap, leftPtr);
	rightKey = node_read_data(myHeap, rightPtr);

	newHeight = CalculateNodeHeight(myHeap, nowPtr);
	node_set_height(myHeap, nowPtr, newHeight);
	
	// get balance factor for this ancestor
	balanceFactor = GetBalance(myHeap, nowPtr);
	// if unbalanced, there are 4 cases


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

ptr_t ProcessNodeDeletion(data_t *myHeap, ptr_t nowPtr){
	ptr_t outputPtr;
	int newHeight;
	signed balanceFactor;
	ptr_t leftPtr, rightPtr;	
	//printf("nowPtr = %d\n",nowPtr);
	if(nowPtr == NULL_PTR){
		outputPtr = nowPtr;
	}else{
		// update height
		newHeight = CalculateNodeHeight(myHeap, nowPtr);
		node_set_height(myHeap, nowPtr, newHeight);
		
		//get balance factor
		balanceFactor = GetBalance(myHeap, nowPtr);
		
		// if unbalanced, there are 4 cases
		leftPtr = node_get_left_pointer(myHeap, nowPtr);
		rightPtr = node_get_right_pointer(myHeap, nowPtr);
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
