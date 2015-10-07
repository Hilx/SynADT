#include "main.h"

/* Application Level */
/* Delete Tree */
ptr_t DeleteTree(int *myHeap, int *stackPtr, ptr_t treePtr){
	int flag_stackIsUsed = 0;
	int flag_stop = 0;
	struct sub_t subResult;
	struct stack_t stackOutput;
	ptr_t currentPtr = treePtr;
	ptr_t returnPtr;
	
	while(flag_stop == 0){
		
		subResult = DeleteTreeSub(myHeap, currentPtr);
		if(subResult.feedback == FB_DONE){
			if(flag_stackIsUsed == 0){ 
				flag_stop = 1;				
				returnPtr = subResult.pointer;
			}else{
				if(stackPtr == NULL){
					flag_stop = 1;
					returnPtr = subResult.pointer;
				}else{
				
					stackOutput = myStack(stackPtr, READ_STACK, 0, 0);
					stackPtr = stackOutput.hdPtr;
					currentPtr = stackOutput.pointer;

					if(stackOutput.operation == GOING_LEFT){
						node_set_left(myHeap, stackOutput.pointer, subResult.pointer);
					}else{
						node_set_right(myHeap, stackOutput.pointer, subResult.pointer);
					}
				}

			}
		}else{
			flag_stackIsUsed = 1;			
			if(subResult.feedback == FB_LEFT){				
				stackOutput = myStack(stackPtr, WRITE_STACK, currentPtr, GOING_LEFT);	
				stackPtr = stackOutput.hdPtr;
				currentPtr = subResult.pointer;

			}else{				
				stackOutput = myStack(stackPtr, WRITE_STACK, currentPtr, GOING_RIGHT);	
				stackPtr = stackOutput.hdPtr;
				currentPtr = subResult.pointer;
			}	
			
		}
	}
	return returnPtr;
}
struct sub_t DeleteTreeSub(int *myHeap, ptr_t treePtr){
	struct sub_t output;
	ptr_t leftPtr = node_get_leftNodePtr(myHeap, treePtr);
	ptr_t rightPtr = node_get_rightNodePtr(myHeap, treePtr);
		if(treePtr == NULL_PTR){
			output.pointer = NULL_PTR;
			output.feedback = FB_DONE;
		}else if(leftPtr == NULL_PTR && rightPtr == NULL_PTR){
			node_delete(treePtr);
			output.pointer = NULL_PTR;
			output.feedback = FB_DONE;
		}else if(leftPtr != NULL_PTR){
			output.pointer = leftPtr;
			output.feedback = FB_LEFT;
		}else if(rightPtr != NULL_PTR){
			output.pointer = rightPtr;
			output.feedback = FB_RIGHT;			
		}	
	return output;
}

/* Search */
ptr_t Search(int *myHeap, int *stackPtr, ptr_t treePtr, int data){
	ptr_t localPtr = treePtr;
	struct sub_t subResult;
	ptr_t outputPtr;
	int flag_found = 0;
	
	subResult = SearchSub(myHeap, localPtr, data);
	while(subResult.pointer != NULL_PTR && flag_found == 0){
		
		
		if(subResult.feedback == FB_DONE){
			outputPtr = subResult.pointer;
			flag_found = 1;
		}else{ 
			if(subResult.feedback == FB_LEFT){
				localPtr = node_get_leftNodePtr(myHeap, localPtr);
			}else{
				localPtr = node_get_rightNodePtr(myHeap, localPtr);
			}		
			subResult = SearchSub(myHeap, localPtr, data);
		}
	}
	return outputPtr;	
}

struct sub_t SearchSub(int *myHeap, ptr_t treePtr, int data){
	struct sub_t output;
	int readData = node_read_data(myHeap, treePtr);
	
	if(data == readData){
		output.pointer = treePtr;
		output.feedback = FB_DONE;
	}else if(data < readData){
		output.feedback = FB_LEFT;	
	}else{
		output.feedback = FB_RIGHT;
	}
	
	return output;
}
/* Create Tree */
ptr_t TreeGen(int*myHeap, int *stackPtr, int NumberOfNodes){
	int i;
	ptr_t root2return = NULL_PTR;
	ptr_t insertResult;
	int inputArray[7] = {5,3,7,2,4,6,8};
	for(i = 0; i < NumberOfNodes; i++){		
		if(i == 0){
			root2return = Insert(myHeap, stackPtr, root2return,inputArray[i]);
		}else{
			insertResult = Insert(myHeap, stackPtr, root2return,inputArray[i]);
		}
		//printTree(myHeap, root2return);
	}
	return root2return;
}

ptr_t Insert(int *myHeap, int *stackPtr, ptr_t treePtr, int data){
	int flag_stackIsUsed = 0;
	int flag_stop = 0;
	struct sub_t subResult;
	struct stack_t stackOutput;
	ptr_t currentPtr = treePtr;
	ptr_t returnPtr;
	
	while(flag_stop == 0){
		
		subResult = InsertSub(myHeap, currentPtr, data);
		if(subResult.feedback == FB_DONE){

			flag_stop = 1;	
			//printf("flag_stop = %d", flag_stop);
			if(flag_stackIsUsed == 0){ 
				/* 	if the node is inserted before the stack is used,
				return the pointer address back. 
				no rectursion has happened, so no need to update 
				left or right pointers. */				
				returnPtr = subResult.pointer;
			}else{

				stackOutput = myStack(stackPtr, READ_STACK, 0, 0);
				stackPtr = stackOutput.hdPtr;

				if(stackOutput.operation == GOING_LEFT){
					node_set_left(myHeap, stackOutput.pointer, subResult.pointer);
				}else{
					node_set_right(myHeap, stackOutput.pointer, subResult.pointer);
				}
				// read stack to clear stack
				while(stackPtr != NULL){
					stackOutput = myStack(stackPtr, READ_STACK, 0, 0);
					stackPtr = stackOutput.hdPtr;
				}
			}
		}else{
			flag_stackIsUsed = 1;
			
			if(subResult.feedback == FB_LEFT){				
				stackOutput = myStack(stackPtr, WRITE_STACK, currentPtr, GOING_LEFT);	
				stackPtr = stackOutput.hdPtr;
				currentPtr = node_get_leftNodePtr(myHeap, currentPtr);

			}else{				
				stackOutput = myStack(stackPtr, WRITE_STACK, currentPtr, GOING_RIGHT);	
				stackPtr = stackOutput.hdPtr;
				currentPtr = node_get_rightNodePtr(myHeap, currentPtr);
			}	
			
		}
	}
	return returnPtr;
}

struct sub_t InsertSub(int *myHeap, ptr_t treePtr, int data){
	data_t nodeData = node_read_data(myHeap, treePtr);
	struct sub_t output;
	if(treePtr == NULL_PTR){
		output.pointer = node_alloc_new(myHeap, data, NULL_PTR, NULL_PTR);
		output.feedback = FB_DONE;
		//return:data_inserted & pointer of new node created
	}else if(data < nodeData){
		output.feedback = FB_LEFT;
		// return:go to the left node
	}else{
		output.feedback = FB_RIGHT;
		// return:go to the right node
	}
	return output;
}


/* Node Operations */
void node_set_left(int *myHeap, ptr_t currentPtr, ptr_t nextPtr){
	next_t offset =  nextPtr - currentPtr;
	node_write_left(myHeap, currentPtr, offset);
}
void node_set_right(int *myHeap, ptr_t currentPtr, ptr_t nextPtr){
	next_t offset =  nextPtr - currentPtr;
	node_write_right(myHeap, currentPtr, offset);
}

ptr_t node_alloc_new(int *myHeap, data_t data, ptr_t leftNodePtr, ptr_t rightNodePtr){
	ptr_t newNodePtr;
	struct node_t newNode;
	newNodePtr = SysMalloc(sizeof(struct node_t));
	newNode.data = data;
	newNode.left = leftNodePtr - newNodePtr;
	newNode.right = rightNodePtr - newNodePtr;
	node_write(myHeap, newNodePtr, newNode);
	return newNodePtr;
}
ptr_t node_get_leftNodePtr(int *myHeap, ptr_t currentNodePtr){
	next_t offset = node_read_left(myHeap, currentNodePtr);
	return currentNodePtr + offset;
}
ptr_t node_get_rightNodePtr(int *myHeap, ptr_t currentNodePtr){
	next_t offset = node_read_right(myHeap, currentNodePtr);
	return currentNodePtr + offset;
}
void node_delete(ptr_t nodePtr){
	SysFree(nodePtr);
}

/* Whole Node Access in Memory */
void node_write(int *myHeap, ptr_t nodePtr, struct node_t nodeIn){
	node_write_data(myHeap, nodePtr, nodeIn.data);
	node_write_left(myHeap, nodePtr, nodeIn.left);
	node_write_right(myHeap, nodePtr, nodeIn.right);	
}
struct node_t node_read(int *myHeap, ptr_t nodePtr){
	struct node_t NodeOut;
	NodeOut.data = node_read_data(myHeap, nodePtr);
	NodeOut.left = node_read_left(myHeap, nodePtr);
	NodeOut.right= node_read_right(myHeap, nodePtr);
	return NodeOut;
}

/* Access Node in Memory (SINGLE ELEMENT) - Write Node */
void node_write_left(int *myHeap, ptr_t nodePtr, next_t offset){
	myHeap[nodePtr+LEFT_OFFSET] = offset;
	
}
void node_write_right(int *myHeap, ptr_t nodePtr, next_t offset){
	myHeap[nodePtr+RIGHT_OFFSET] = offset;
}
void node_write_data(int *myHeap, ptr_t nodePtr, data_t data){
	myHeap[nodePtr+DATA_OFFSET] = data;
}

/* Access Node in Memory (SINGLE ELEMENT) - Read Node */
next_t node_read_left(int *myHeap, ptr_t nodePtr){
	return myHeap[nodePtr + LEFT_OFFSET];
}
next_t node_read_right(int *myHeap, ptr_t nodePtr){
	return myHeap[nodePtr + RIGHT_OFFSET];
}
data_t node_read_data(int *myHeap, ptr_t nodePtr){
	return myHeap[nodePtr + DATA_OFFSET];
}
/* software only */
void printTree(int *myHeap, ptr_t nowPtr){
	printf("Printing the tree now [ ");
	printValue(myHeap, nowPtr);
	printf(" ] Done printing.\n");
}
void printValue(int *myHeap, ptr_t nowPtr){
	if(nowPtr != NULL_PTR){
		printValue(myHeap, node_get_leftNodePtr(myHeap, nowPtr));
		printf("%d ", node_read_data(myHeap, nowPtr));
		printValue(myHeap, node_get_rightNodePtr(myHeap, nowPtr));
	}
}
