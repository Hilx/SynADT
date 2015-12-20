#include "SynADT.h"

/* Application Level */
/* Check then Insert */
ptr_t Check_thenInsert(int *myHeap, int *fixedStack, int stackPtr_avl, ptr_t rootPtr, int key){	
	struct search_t serResult = Search(myHeap, fixedStack, stackPtr_avl, rootPtr, key);	
	if(serResult.flag_failed == 1){ // if search failed, insert
		struct ptrBundle_t newBundle = Insert(myHeap, fixedStack, stackPtr_avl, rootPtr, key, NULL_PTR, NULL_PTR);
		rootPtr = newBundle.root;
	}
	return rootPtr;
}
/* update */
ptr_t UpdateNode(int *myHeap, int *fixedStack, int stackPtr_avl, ptr_t rootPtr, int oldKey, int newKey){
	rootPtr = DeleteTreeNode(myHeap, fixedStack, stackPtr_avl, rootPtr, oldKey); 
	struct ptrBundle_t newBundle = Insert(myHeap, fixedStack, stackPtr_avl, rootPtr, newKey, NULL_PTR, NULL_PTR);
	return newBundle.root;
}

/* Delete Node */
ptr_t DeleteTreeNode(int *myHeap, int *fixedStack, int stackPtr_avl, ptr_t rootPtr, int key){
	struct search_t node2delete = Search(myHeap, fixedStack, stackPtr_avl, rootPtr, key);
	if(node2delete.flag_failed == 0){
		// ----------------------------------- Deletion -----------------------------------
		
		ptr_t nodePtr = node2delete.nodePtr;
		ptr_t leftPtr = node_get_leftNodePtr(myHeap, nodePtr);
		ptr_t rightPtr = node_get_rightNodePtr(myHeap, nodePtr);
		struct node_t node2move;
		struct ptrBundle_t temp_insert_result;
		ptr_t tempPtr;
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
					ptr_t right_leftPtr = node_get_leftNodePtr(myHeap, rightPtr);		
					ptr_t right_rightPtr = node_get_rightNodePtr(myHeap, rightPtr);					
					temp_insert_result = Insert(myHeap, fixedStack, stackPtr_avl, rootPtr, rightNodeData, right_leftPtr, right_rightPtr);	
					tempPtr = temp_insert_result.nodePtr;
				}
			}			
			// ################## [then starts to process nodePtr] ##################
		}else if(rightPtr != NULL_PTR){
			node_write_data(myHeap, nodePtr, node_read_data(myHeap, rightPtr));
			ptr_t right_leftPtr = node_get_leftNodePtr(myHeap, rightPtr);
			node_set_left(myHeap, nodePtr, right_leftPtr);
			ptr_t right_rightPtr = node_get_rightNodePtr(myHeap, rightPtr);
			node_set_right(myHeap, nodePtr, right_rightPtr);			
			node_delete(rightPtr);	
			// ################## [then starts to process nodePtr] ##################			
		}else{ // no children nodes
			// need to update the parent node's pointer

			node_delete(nodePtr);	
			/*
			if(node2delete.direction == GOING_LEFT){
				node_set_left(myHeap, node2delete.parentPtr, NULL_PTR);	
			}else if(node2delete.direction == GOING_RIGHT){// right pointer need to be updated
				node_set_right(myHeap, node2delete.parentPtr, NULL_PTR);	
			}else{
				//update root;
				rootPtr = NULL_PTR;
			}
			*/
			nodePtr = NULL_PTR;
			// ################## [then starts to process nodePtr] ##################
		}
		
		// ----------------------------------- Balancing -----------------------------------
		ptr_t nodePtr_updated;
		if(nodePtr != NULL_PTR){
			nodePtr_updated = ProcessNodeDeletion(myHeap, nodePtr);
		}else{
			nodePtr_updated = NULL_PTR;
		}		
		
		int hdPtr_avl_local = node2delete.stackPtr_avl;
		while(hdPtr_avl_local > 0){
			struct stack_t stackOutput = avlStack(fixedStack, hdPtr_avl_local, READ_STACK, 0, 0);
			hdPtr_avl_local = stackOutput.hdPtr_avl; // update stack pointer
			
			if(nodePtr_updated == NULL_PTR || nodePtr_updated != nodePtr){
				if(stackOutput.operation == GOING_LEFT){
					node_set_left(myHeap, stackOutput.pointer, nodePtr_updated);
				}else{
					node_set_right(myHeap, stackOutput.pointer, nodePtr_updated);
				}		
			}
			nodePtr = stackOutput.pointer;
			nodePtr_updated = ProcessNodeDeletion(myHeap, nodePtr);
		}
		rootPtr = nodePtr_updated;
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
				if(stackPtr_avl == 0){	
					flag_stop = 1;
					returnPtr = subResult.pointer;
				}else{
					stackOutput = avlStack(fixedStack, stackPtr_avl, READ_STACK, 0, 0);
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
				stackOutput = avlStack(fixedStack, stackPtr_avl, WRITE_STACK, currentPtr, GOING_LEFT);
				stackPtr_avl = stackOutput.hdPtr_avl;
				currentPtr = subResult.pointer;

			}else{				
				stackOutput = avlStack(fixedStack, stackPtr_avl, WRITE_STACK, currentPtr, GOING_RIGHT);	
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
	int flag_stackIsUsed = 0;
	
	output.flag_failed = 0;
	output.used_stack = 0; 
	output.direction = 7;
	subResult = SearchSub(myHeap, localPtr, data);
	while(subResult.pointer != NULL_PTR && flag_found == 0){
		
		if(subResult.feedback == FB_DONE){
			output.nodePtr = subResult.pointer;
			flag_found = 1;
			if(flag_stackIsUsed ==  1){
				//stackOutput = avlStack(fixedStack, stackPtr_avl, READ_STACK, 0, 0);
				//output.stackPtr_avl = stackOutput.hdPtr_avl;
				output.stackPtr_avl = stackPtr_avl;
				output.used_stack = 1;
			}else{
				output.stackPtr_avl = 0;
			}
		}else{
			flag_stackIsUsed = 1;
			
			output.parentPtr = localPtr;
			if(subResult.feedback == FB_LEFT){
				stackOutput = avlStack(fixedStack, stackPtr_avl, WRITE_STACK, localPtr, GOING_LEFT);
				stackPtr_avl = stackOutput.hdPtr_avl;
				localPtr = node_get_leftNodePtr(myHeap, localPtr);
				//output.direction = GOING_LEFT;
			}else{
				stackOutput = avlStack(fixedStack, stackPtr_avl, WRITE_STACK, localPtr, GOING_RIGHT);
				stackPtr_avl = stackOutput.hdPtr_avl;
				localPtr = node_get_rightNodePtr(myHeap, localPtr);

				//output.direction = GOING_RIGHT;
			}		
			if(localPtr == NULL_PTR){
				output.flag_failed = 1;
				flag_found = 1;
			}else{
				subResult = SearchSub(myHeap, localPtr, data);
			}
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
	int inputArray[9] = {5,3,7,2,4,6,8,7,2};
	for(i = 0; i < NumberOfNodes; i++){		
		result = Insert(myHeap, fixedStack, stackPtr_avl, root,inputArray[i], NULL_PTR, NULL_PTR);
		root = result.root;
	}
	return root;
}

struct ptrBundle_t Insert(int *myHeap, int *fixedStack, int stackPtr_avl, ptr_t treePtr, int data, ptr_t new_leftPtr, ptr_t new_rightPtr){
	int flag_stackIsUsed = 0;
	int flag_stop = 0;
	struct sub_t subResult;
	struct stack_t stackOutput;
	ptr_t currentPtr = treePtr;
	ptr_t returnPtr;
	ptr_t nowPtr, nowPtr_new;
	ptr_t leftPtr, rightPtr, outputPtr, tempPtr;
	
	struct ptrBundle_t output;
	int stackPtr_avl_original = stackPtr_avl;

	while(flag_stop == 0){
		subResult = InsertSub(myHeap, currentPtr, data, new_leftPtr, new_rightPtr);
		if(subResult.feedback == FB_DONE){
			output.nodePtr = subResult.pointer;
			flag_stop = 1;	
			if(flag_stackIsUsed == 0){ 
				output.root = subResult.pointer;				
			}else{
				stackOutput = avlStack(fixedStack, stackPtr_avl, READ_STACK, 0, 0);				
				stackPtr_avl = stackOutput.hdPtr_avl;
				
				if(subResult.flag_same_key == 0){
					if(stackOutput.operation == GOING_LEFT){
						node_set_left(myHeap, stackOutput.pointer, subResult.pointer);
					}else{
						node_set_right(myHeap, stackOutput.pointer, subResult.pointer);
					}
					
					// read stack to clear stack
					while(stackPtr_avl != stackPtr_avl_original){
						nowPtr = stackOutput.pointer;
						nowPtr_new = ProcessNodeInsertion(myHeap, nowPtr, data);		
						stackOutput = avlStack(fixedStack, stackPtr_avl, READ_STACK, 0, 0);
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
				}else{
					output.root = treePtr;
				}				
			}
		}else{
			flag_stackIsUsed = 1;
			
			if(subResult.feedback == FB_LEFT){
				stackOutput = avlStack(fixedStack, stackPtr_avl, WRITE_STACK, currentPtr, GOING_LEFT);
				stackPtr_avl = stackOutput.hdPtr_avl;
				currentPtr = node_get_leftNodePtr(myHeap, currentPtr);
			}else{
				stackOutput = avlStack(fixedStack, stackPtr_avl, WRITE_STACK, currentPtr, GOING_RIGHT);
				stackPtr_avl = stackOutput.hdPtr_avl;
				currentPtr = node_get_rightNodePtr(myHeap, currentPtr);
			}				
		}
	}
	return output;
}

struct sub_t InsertSub(int *myHeap, ptr_t treePtr, int data , ptr_t new_leftPtr, ptr_t new_rightPtr){
	data_t nodeData = node_read_data(myHeap, treePtr);		
	struct sub_t output;
	output.flag_same_key = 0;
	if(treePtr == NULL_PTR){
		ptr_t tempPtr = node_alloc_new(myHeap, data, new_leftPtr, new_rightPtr);
		output.pointer = ProcessNodeInsertion(myHeap, tempPtr, data);
		output.feedback = FB_DONE;
		//return:data_inserted & pointer of new node created
	}else if(data < nodeData){
		output.feedback = FB_LEFT;
		// return:go to the left node
	}else if (data > nodeData){
		output.feedback = FB_RIGHT;
		// return:go to the right node
	}else if(data == nodeData){
		output.pointer = treePtr;
		output.feedback = FB_DONE;
		output.flag_same_key = 1;		
	}
	return output;
}