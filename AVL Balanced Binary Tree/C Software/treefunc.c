#include "main.h"

/* Application Level */
/* update */
ptr_t UpdateNode(int *myHeap, int *fixedStack, int stackPtr_avl, ptr_t rootPtr, int oldKey, int newKey){
	rootPtr = DeleteTreeNode(myHeap, fixedStack, stackPtr_avl, rootPtr, oldKey); // need to modify this so that it updates the height info in the tree nodes
	struct ptrBundle_t newBundle = Insert(myHeap, fixedStack, stackPtr_avl, rootPtr, newKey);	
	return newBundle.root;
}

/* Delete Node */
ptr_t DeleteTreeNode(int *myHeap, int *fixedStack, int stackPtr_avl, ptr_t rootPtr, int key){
	struct search_t node2delete = Search(myHeap, fixedStack, stackPtr_avl, rootPtr, key);
	ptr_t nodePtr = node2delete.nodePtr;
	//stackPtr = node2delete.stackPtr;
	stackPtr_avl = node2delete.stackPtr_avl;
	ptr_t leftPtr = node_get_leftNodePtr(myHeap, nodePtr);
	ptr_t rightPtr = node_get_rightNodePtr(myHeap, nodePtr);
	struct node_t node2move;
	ptr_t tempPtr;
	
	struct ptrBundle_t insertResult;
	ptr_t nowPtr, nowPtr_new;
	struct stack_t stackOutput;

	/* Deletion */
	if(leftPtr != NULL_PTR){
		// copy contents of leftPtr to nodePtr, then free nodePtr		
		node_write_data(myHeap, nodePtr, node_read_data(myHeap, leftPtr));
		ptr_t left_leftPtr = node_get_leftNodePtr(myHeap, leftPtr);
		node_set_left(myHeap, nodePtr, left_leftPtr);
		ptr_t left_rightPtr = node_get_rightNodePtr(myHeap, leftPtr);
		node_set_right(myHeap, nodePtr, left_rightPtr);

		node_delete(leftPtr);
		if(rightPtr != NULL_PTR){
			
			if(left_rightPtr == NULL_PTR){
				node_set_right(myHeap, nodePtr, rightPtr);				
			}else{
				//insert new node with rightPtr node key value
				int rightNodeData = node_read_data(myHeap, rightPtr);				
				insertResult = Insert(myHeap, fixedStack, stackPtr_avl, rootPtr,rightNodeData);				
				node_write_data(myHeap, insertResult.nodePtr, rightNodeData);
				ptr_t right_leftPtr = node_get_leftNodePtr(myHeap, rightPtr);
				node_set_left(myHeap, insertResult.nodePtr, right_leftPtr);
				ptr_t right_rightPtr = node_get_rightNodePtr(myHeap, rightPtr);
				node_set_right(myHeap, insertResult.nodePtr, right_rightPtr);
				// update the new node's height
				
				tempPtr = ProcessNodeDeletion(myHeap, insertResult.nodePtr);				
			}
		}
	}else if(rightPtr != NULL_PTR){
		node_write_data(myHeap, nodePtr, node_read_data(myHeap, rightPtr));
		ptr_t right_leftPtr = node_get_leftNodePtr(myHeap, rightPtr);
		node_set_left(myHeap, nodePtr, right_leftPtr);
		ptr_t right_rightPtr = node_get_rightNodePtr(myHeap, rightPtr);
		node_set_right(myHeap, nodePtr, right_rightPtr);			
		node_delete(rightPtr);		
	}else{ // no children nodes
		// need to update the parent node's pointer

		node_delete(nodePtr);		
		if(node2delete.direction == GOING_LEFT){
			node_set_left(myHeap, node2delete.parentPtr, NULL_PTR);	
		}else if(node2delete.direction == GOING_RIGHT){// right pointer need to be updated
			node_set_right(myHeap, node2delete.parentPtr, NULL_PTR);	
		}else{
			//update root;
			rootPtr = NULL_PTR;
		}
	}
	
	/* Updating height */
	
	//while(stackPtr != NULL){
	while(stackPtr_avl != 0){
		//stackOutput = myStack(stackPtr, READ_STACK, 0, 0);
		stackOutput = avlStack(fixedStack, stackPtr_avl, READ_STACK, 0, 0);
		
		//stackPtr = stackOutput.hdPtr;
		stackPtr_avl = stackOutput.hdPtr_avl;
		
		nowPtr = stackOutput.pointer;
		nowPtr_new = ProcessNodeDeletion(myHeap, nowPtr);							

		if(nowPtr_new != nowPtr){
			if(stackOutput.operation == GOING_LEFT){
				node_set_left(myHeap, stackOutput.pointer, nowPtr_new);
			}else{
				node_set_right(myHeap, stackOutput.pointer, nowPtr_new);
			}					
		}
	}
	
	return rootPtr;
}

/* Delete Tree */
ptr_t DeleteTree(int *myHeap, int *fixedStack, int stackPtr_avl, ptr_t treePtr){
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
				//if(stackPtr == NULL){
				if(stackPtr_avl == 0){	
					flag_stop = 1;
					returnPtr = subResult.pointer;
				}else{
					
					//stackOutput = myStack(stackPtr, READ_STACK, 0, 0);
					stackOutput = avlStack(fixedStack, stackPtr_avl, READ_STACK, 0, 0);
					//stackPtr = stackOutput.hdPtr;
					stackPtr_avl = stackOutput.hdPtr_avl;
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
				//stackOutput = myStack(stackPtr, WRITE_STACK, currentPtr, GOING_LEFT);	
				stackOutput = avlStack(fixedStack, stackPtr_avl, WRITE_STACK, currentPtr, GOING_LEFT);
				//stackPtr = stackOutput.hdPtr;
				stackPtr_avl = stackOutput.hdPtr_avl;
				currentPtr = subResult.pointer;

			}else{				
				//stackOutput = myStack(stackPtr, WRITE_STACK, currentPtr, GOING_RIGHT);	
				stackOutput = avlStack(fixedStack, stackPtr_avl, WRITE_STACK, currentPtr, GOING_RIGHT);				
				//stackPtr = stackOutput.hdPtr;
				stackPtr_avl = stackOutput.hdPtr_avl;
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
struct search_t Search(int *myHeap, int *fixedStack, int stackPtr_avl, ptr_t treePtr, int data){
	ptr_t localPtr = treePtr;
	struct sub_t subResult;
	struct search_t output;
	int flag_found = 0;	
	struct stack_t stackOutput; // stack

	output.direction = 7;
	subResult = SearchSub(myHeap, localPtr, data);
	while(subResult.pointer != NULL_PTR && flag_found == 0){
		
		if(subResult.feedback == FB_DONE){
			output.nodePtr = subResult.pointer;
			flag_found = 1;
			//stackOutput = myStack(stackPtr, READ_STACK, 0, 0);
			stackOutput = avlStack(fixedStack, stackPtr_avl, READ_STACK, 0, 0);
			//output.stackPtr = stackOutput.hdPtr;	
			output.stackPtr_avl = stackOutput.hdPtr_avl;
		}else{
			output.parentPtr = localPtr;
			if(subResult.feedback == FB_LEFT){	
				//stackOutput = myStack(stackPtr, WRITE_STACK, localPtr, GOING_LEFT);	
				stackOutput = avlStack(fixedStack, stackPtr_avl, WRITE_STACK, localPtr, GOING_LEFT);
				//stackPtr = stackOutput.hdPtr;
				stackPtr_avl = stackOutput.hdPtr_avl;
				localPtr = node_get_leftNodePtr(myHeap, localPtr);
				output.direction = GOING_LEFT;
			}else{
				//stackOutput = myStack(stackPtr, WRITE_STACK, localPtr, GOING_RIGHT);
				stackOutput = avlStack(fixedStack, stackPtr_avl, WRITE_STACK, localPtr, GOING_RIGHT);
				//stackPtr = stackOutput.hdPtr;
				stackPtr_avl = stackOutput.hdPtr_avl;
				localPtr = node_get_rightNodePtr(myHeap, localPtr);
				output.direction = GOING_RIGHT;
			}		
			subResult = SearchSub(myHeap, localPtr, data);
		}		
	}
	return output;	
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
ptr_t TreeGen(int*myHeap, int *fixedStack, int stackPtr_avl, int NumberOfNodes){
	int i;
	ptr_t root = NULL_PTR;
	struct ptrBundle_t result;
	int inputArray[7] = {5,3,7,2,4,6,8};
	for(i = 0; i < NumberOfNodes; i++){		
		result = Insert(myHeap, fixedStack, stackPtr_avl, root,i);
		root = result.root;
	}
	return root;
}

struct ptrBundle_t Insert(int *myHeap, int *fixedStack, int stackPtr_avl, ptr_t treePtr, int data){
	int flag_stackIsUsed = 0;
	int flag_stop = 0;
	struct sub_t subResult;
	struct stack_t stackOutput;
	ptr_t currentPtr = treePtr;
	ptr_t returnPtr;
	ptr_t nowPtr, nowPtr_new;
	ptr_t leftPtr, rightPtr, outputPtr, tempPtr;
	
	struct ptrBundle_t output;
	//int *stackPtr_original = stackPtr;
	int stackPtr_avl_original = stackPtr_avl;

	while(flag_stop == 0){
		subResult = InsertSub(myHeap, currentPtr, data);
		if(subResult.feedback == FB_DONE){
			output.nodePtr = subResult.pointer;
			flag_stop = 1;	
			if(flag_stackIsUsed == 0){ 
				//returnPtr = subResult.pointer;
				output.root = subResult.pointer;				
			}else{

				//stackOutput = myStack(stackPtr, READ_STACK, 0, 0);
				stackOutput = avlStack(fixedStack, stackPtr_avl, READ_STACK, 0, 0);
				
				//stackPtr = stackOutput.hdPtr;
				stackPtr_avl = stackOutput.hdPtr_avl;
				
				if(stackOutput.operation == GOING_LEFT){
					node_set_left(myHeap, stackOutput.pointer, subResult.pointer);
				}else{
					node_set_right(myHeap, stackOutput.pointer, subResult.pointer);
				}
				// read stack to clear stack
				while(stackPtr_avl != stackPtr_avl_original){
					nowPtr = stackOutput.pointer;
					nowPtr_new = ProcessNodeInsertion(myHeap, nowPtr, data);		
					//stackOutput = myStack(stackPtr, stackPtr_avl, 0, 0);
					stackOutput = avlStack(fixedStack, stackPtr_avl, READ_STACK, 0, 0);
					//stackPtr = stackOutput.hdPtr;
					stackPtr_avl = stackOutput.hdPtr_avl;
					if(nowPtr_new != nowPtr){
						if(stackOutput.operation == GOING_LEFT){
							node_set_left(myHeap, stackOutput.pointer, nowPtr_new);
						}else{
							node_set_right(myHeap, stackOutput.pointer, nowPtr_new);
						}					
					}
				}
				output.root = ProcessNodeInsertion(myHeap, stackOutput.pointer, data);			
			}
		}else{
			flag_stackIsUsed = 1;
			
			if(subResult.feedback == FB_LEFT){				
				//stackOutput = myStack(stackPtr, WRITE_STACK, currentPtr, GOING_LEFT);	
				stackOutput = avlStack(fixedStack, stackPtr_avl, WRITE_STACK, currentPtr, GOING_LEFT);
				//stackPtr = stackOutput.hdPtr;
				stackPtr_avl = stackOutput.hdPtr_avl;
				currentPtr = node_get_leftNodePtr(myHeap, currentPtr);

			}else{				
				//stackOutput = myStack(stackPtr, WRITE_STACK, currentPtr, GOING_RIGHT);	
				stackOutput = avlStack(fixedStack, stackPtr_avl, WRITE_STACK, currentPtr, GOING_RIGHT);
				//stackPtr = stackOutput.hdPtr;
				stackPtr_avl = stackOutput.hdPtr_avl;
				currentPtr = node_get_rightNodePtr(myHeap, currentPtr);
			}				
		}
	}
	return output;
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
void node_set_height(int *myHeap, ptr_t currentPtr, int height){
	node_write_height(myHeap, currentPtr, height);
}

ptr_t node_alloc_new(int *myHeap, data_t data, ptr_t leftNodePtr, ptr_t rightNodePtr){
	ptr_t newNodePtr;
	struct node_t newNode;
	newNodePtr = SysMalloc(sizeof(struct node_t));
	newNode.data = data;
	newNode.left = leftNodePtr - newNodePtr;
	newNode.right = rightNodePtr - newNodePtr;
	newNode.height = 0;
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
	node_write_height(myHeap, nodePtr, nodeIn.height);
}
struct node_t node_read(int *myHeap, ptr_t nodePtr){
	struct node_t NodeOut;
	NodeOut.data = node_read_data(myHeap, nodePtr);
	NodeOut.left = node_read_left(myHeap, nodePtr);
	NodeOut.right= node_read_right(myHeap, nodePtr);
	NodeOut.height = node_read_height(myHeap, nodePtr);
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
void node_write_height(int *myHeap, ptr_t nodePtr, int height){
	myHeap[nodePtr+HEIGHT_OFFSET] = height;
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
int node_read_height(int *myHeap, ptr_t nodePtr){
	return myHeap[nodePtr + HEIGHT_OFFSET];
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
