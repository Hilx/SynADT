#include "SynADT_header.h"

/* Application Level */
/* Create Tree */
ptr_t TreeGen(data_t *Master2Mem, data_t *Master2SysAlloc, data_t *fixedStack, ptr_t stackPtr_avl, int NumberOfNodes){
	int i;
	ptr_t root = NULL_PTR;
	struct ptrBundle_t result;
	int inputArray[7] = {5,3,7,2,4,6,8};

	for(i = 0; i < NumberOfNodes; i++){		
		result = Insert(Master2Mem, Master2SysAlloc, fixedStack, stackPtr_avl, root, i);
		root = result.root;
	}
	return root;
}
struct ptrBundle_t Insert(data_t *Master2Mem, data_t *Master2SysAlloc, data_t *fixedStack, ptr_t stackPtr_avl, ptr_t treePtr, int data){
	int flag_stackIsUsed = 0;
	int flag_stop = 0;
	struct sub_t subResult;
	struct stack_t stackOutput;
	ptr_t currentPtr = treePtr;
	//ptr_t returnPtr;
	ptr_t nowPtr, nowPtr_new;
	ptr_t leftPtr, rightPtr, outputPtr, tempPtr;
	
	struct ptrBundle_t output;
	int stackPtr_avl_original = stackPtr_avl;	
	
	while(flag_stop == 0){
		subResult = InsertSub(Master2Mem, Master2SysAlloc, currentPtr, data);
		if(subResult.feedback == FB_DONE){
			output.nodePtr = subResult.pointer;
			flag_stop = 1;	
			if(flag_stackIsUsed == 0){ 
				output.root = subResult.pointer;
			}else{
				stackOutput = staticStack(fixedStack, stackPtr_avl, READ_STACK, 0, 0);
				stackPtr_avl = stackOutput.hdPtr_avl;

				if(stackOutput.operation == GOING_LEFT){
					tree_node_set_left(Master2Mem, stackOutput.pointer, subResult.pointer);
				}else{
					tree_node_set_right(Master2Mem, stackOutput.pointer, subResult.pointer);
				}
				// read stack to clear stack
				while(stackPtr_avl != stackPtr_avl_original){
					nowPtr = stackOutput.pointer;
					nowPtr_new = ProcessNodeInsertion(Master2Mem, nowPtr, data);
					stackOutput = staticStack(fixedStack, stackPtr_avl, READ_STACK, 0, 0);
					stackPtr_avl = stackOutput.hdPtr_avl;
					if(nowPtr_new != nowPtr){
						if(stackOutput.operation == GOING_LEFT){
							tree_node_set_left(Master2Mem, stackOutput.pointer, nowPtr_new);
						}else{
							tree_node_set_right(Master2Mem, stackOutput.pointer, nowPtr_new);
						}					
					}					
				}
				output.root = ProcessNodeInsertion(Master2Mem, stackOutput.pointer, data);
			}
		}else{
			flag_stackIsUsed = 1;
			
			if(subResult.feedback == FB_LEFT){				
				stackOutput = staticStack(fixedStack, stackPtr_avl, WRITE_STACK, currentPtr, GOING_LEFT);	
				stackPtr_avl = stackOutput.hdPtr_avl;
				currentPtr = tree_node_get_left_pointer(Master2Mem, currentPtr);
			}else{				
				stackOutput = staticStack(fixedStack, stackPtr_avl, WRITE_STACK, currentPtr, GOING_RIGHT);
				stackPtr_avl = stackOutput.hdPtr_avl;
				currentPtr = tree_node_get_right_pointer(Master2Mem, currentPtr);
			}				
		}
	}
	return output;
}
struct sub_t InsertSub(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t treePtr, data_t data){
	data_t nodeData = tree_node_read_data(Master2Mem, treePtr);
	struct sub_t output;
	if(treePtr == NULL_PTR){
		output.pointer = tree_node_alloc_new(Master2Mem, Master2SysAlloc, data, NULL_PTR, NULL_PTR);
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

/* Search */
struct search_t Search(data_t *Master2Mem, data_t *fixedStack, ptr_t stackPtr_avl, ptr_t treePtr, int data){
	ptr_t localPtr = treePtr;
	struct sub_t subResult;
	struct search_t output;
	int flag_found = 0;
	struct stack_t stackOutput;
	
	output.direction = 7;
	subResult = SearchSub(Master2Mem, localPtr, data);
	while(subResult.pointer != NULL_PTR && flag_found == 0){	
		if(subResult.feedback == FB_DONE){
			output.nodePtr = subResult.pointer;
			flag_found = 1;
			stackOutput = staticStack(fixedStack, stackPtr_avl, READ_STACK, 0, 0);
			output.stackPtr_avl = stackOutput.hdPtr_avl;
		}else{
			output.parentPtr = localPtr;
			if(subResult.feedback == FB_LEFT){			
				stackOutput = staticStack(fixedStack, stackPtr_avl, WRITE_STACK, localPtr, GOING_LEFT);
				stackPtr_avl = stackOutput.hdPtr_avl;
				localPtr = tree_node_get_left_pointer(Master2Mem, localPtr);
				output.direction = GOING_LEFT;
			}else{
				stackOutput = staticStack(fixedStack, stackPtr_avl, WRITE_STACK, localPtr, GOING_RIGHT);
				stackPtr_avl = stackOutput.hdPtr_avl;
				localPtr = tree_node_get_right_pointer(Master2Mem, localPtr);
				output.direction = GOING_RIGHT;
			}		
			subResult = SearchSub(Master2Mem, localPtr, data);
		}
	}
	return output;	
}
struct sub_t SearchSub(data_t *Master2Mem, ptr_t treePtr, int data){
	struct sub_t output;
	int readData = tree_node_read_data(Master2Mem, treePtr);
	
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

/* Delete Tree */
ptr_t DeleteTree(data_t *Master2Mem, data_t *Master2SysAlloc, data_t *fixedStack, ptr_t stackPtr_avl, ptr_t treePtr){
	int flag_stackIsUsed = 0;
	int flag_stop = 0;
	struct sub_t subResult;
	struct stack_t stackOutput;
	ptr_t currentPtr = treePtr;
	ptr_t returnPtr;
	
	while(flag_stop == 0){
		
		subResult = DeleteTreeSub(Master2Mem, Master2SysAlloc, currentPtr);
		if(subResult.feedback == FB_DONE){
			if(flag_stackIsUsed == 0){ 
				flag_stop = 1;				
				returnPtr = subResult.pointer;
			}else{
				if(stackPtr_avl == NULL_PTR){
					flag_stop = 1;
					returnPtr = subResult.pointer;
				}else{				
					stackOutput = staticStack(fixedStack, stackPtr_avl, READ_STACK, 0, 0);
					stackPtr_avl = stackOutput.hdPtr_avl;
					currentPtr = stackOutput.pointer;

					if(stackOutput.operation == GOING_LEFT){
						tree_node_set_left(Master2Mem, stackOutput.pointer, subResult.pointer);
					}else{
						tree_node_set_right(Master2Mem, stackOutput.pointer, subResult.pointer);
					}
				}

			}
		}else{
			flag_stackIsUsed = 1;			
			if(subResult.feedback == FB_LEFT){				
				stackOutput = staticStack(fixedStack, stackPtr_avl, WRITE_STACK, currentPtr, GOING_LEFT);	
				stackPtr_avl = stackOutput.hdPtr_avl;
				currentPtr = subResult.pointer;

			}else{				
				stackOutput = staticStack(fixedStack, stackPtr_avl, WRITE_STACK, currentPtr, GOING_RIGHT);	
				stackPtr_avl = stackOutput.hdPtr_avl;
				currentPtr = subResult.pointer;
			}	
			
		}
	}
	return returnPtr;
}
struct sub_t DeleteTreeSub(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t treePtr){
	struct sub_t output;
	ptr_t leftPtr = tree_node_get_left_pointer(Master2Mem, treePtr);
	ptr_t rightPtr = tree_node_get_right_pointer(Master2Mem, treePtr);
		if(treePtr == NULL_PTR){
			output.pointer = NULL_PTR;
			output.feedback = FB_DONE;
		}else if(leftPtr == NULL_PTR && rightPtr == NULL_PTR){
			tree_node_delete(Master2SysAlloc, treePtr);
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

/* Delete Node */
ptr_t DeleteTreeNode(data_t *Master2Mem, data_t *Master2SysAlloc,data_t *fixedStack, ptr_t stackPtr_avl, ptr_t rootPtr, int key){
	struct search_t node2delete = Search(Master2Mem, fixedStack, stackPtr_avl, rootPtr, key);
	ptr_t nodePtr = node2delete.nodePtr;
	stackPtr_avl = node2delete.nodePtr;
	ptr_t leftPtr = tree_node_get_left_pointer(Master2Mem, nodePtr);
	ptr_t rightPtr = tree_node_get_right_pointer(Master2Mem, nodePtr);
	struct tree_ode2move;
	ptr_t tempPtr;
	
	struct ptrBundle_t insertResult;
	ptr_t nowPtr, nowPtr_new;
	struct stack_t stackOutput;
	
	if(leftPtr != NULL_PTR){
		// copy contents of leftPtr to nodePtr, then free nodePtr		
		tree_node_write_data(Master2Mem, nodePtr, tree_node_read_data(Master2Mem, leftPtr));
		ptr_t left_leftPtr = tree_node_get_left_pointer(Master2Mem, leftPtr);
		tree_node_set_left(Master2Mem, nodePtr, left_leftPtr);
		ptr_t left_rightPtr = tree_node_get_right_pointer(Master2Mem, leftPtr);
		tree_node_set_right(Master2Mem, nodePtr, left_rightPtr);

		tree_node_delete(Master2SysAlloc, leftPtr);
		if(rightPtr != NULL_PTR){
			
			if(left_rightPtr == NULL_PTR){
				tree_node_set_right(Master2Mem, nodePtr, rightPtr);				
			}else{
				//insert new node with rightPtr node key value
				int rightNodeData = tree_node_read_data(Master2Mem, rightPtr);				
				insertResult = Insert(Master2Mem, Master2SysAlloc, fixedStack, stackPtr_avl, rootPtr, rightNodeData);				
				tree_node_write_data(Master2Mem, tempPtr, rightNodeData);
				ptr_t right_leftPtr = tree_node_get_left_pointer(Master2Mem, rightPtr);
				tree_node_set_left(Master2Mem, tempPtr, right_leftPtr);
				ptr_t right_rightPtr = tree_node_get_right_pointer(Master2Mem, rightPtr);
				tree_node_set_right(Master2Mem, tempPtr, right_rightPtr);
				tempPtr = ProcessNodeDeletion(Master2Mem, insertResult.nodePtr);
			}
		}
	}else if(rightPtr != NULL_PTR){
		tree_node_write_data(Master2Mem, nodePtr, tree_node_read_data(Master2Mem, rightPtr));
		ptr_t right_leftPtr = tree_node_get_left_pointer(Master2Mem, rightPtr);
		tree_node_set_left(Master2Mem, nodePtr, right_leftPtr);
		ptr_t right_rightPtr = tree_node_get_right_pointer(Master2Mem, rightPtr);
		tree_node_set_right(Master2Mem, nodePtr, right_rightPtr);			
		tree_node_delete(Master2SysAlloc, rightPtr);		
	}else{ // no children nodes
		// need to update the parent node's pointer

		tree_node_delete(Master2SysAlloc, nodePtr);		
		if(node2delete.direction == GOING_LEFT){
			tree_node_set_left(Master2Mem, node2delete.parentPtr, NULL_PTR);	
		}else if(node2delete.direction == GOING_RIGHT){// right pointer need to be updated
			tree_node_set_right(Master2Mem, node2delete.parentPtr, NULL_PTR);	
		}else{
			//update root;
			rootPtr = NULL_PTR;
		}
	}
	
	/* Updateing height */
	while(stackPtr_avl != 0){
		stackOutput = staticStack(fixedStack, stackPtr_avl, READ_STACK, 0, 0);
		stackPtr_avl = stackOutput.hdPtr_avl;
		
		nowPtr = stackOutput.pointer;
		nowPtr_new = ProcessNodeDeletion(Master2Mem, nowPtr);							

		if(nowPtr_new != nowPtr){
			if(stackOutput.operation == GOING_LEFT){
				tree_node_set_left(Master2Mem, stackOutput.pointer, nowPtr_new);
			}else{
				tree_node_set_right(Master2Mem, stackOutput.pointer, nowPtr_new);
			}					
		}
	}
		
	return rootPtr;
}

/* update */
ptr_t UpdateNode(data_t *Master2Mem, data_t *Master2SysAlloc, data_t *fixedStack, ptr_t stackPtr_avl, ptr_t rootPtr, int oldKey, int newKey){
	rootPtr = DeleteTreeNode(Master2Mem, Master2SysAlloc, fixedStack, stackPtr_avl, rootPtr, oldKey);
	struct ptrBundle_t newBundle = Insert(Master2Mem, Master2SysAlloc, fixedStack, stackPtr_avl, rootPtr, newKey);	
	return newBundle.root;
}
