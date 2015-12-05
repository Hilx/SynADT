#include "SynADT.h"

/* Application Level */
/* Check then Insert */
ptr_t Check_thenInsert(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t stackPtr, ptr_t rootPtr, int key){	
	struct search_t serResult = Search(Master2Mem, rootPtr, key);	
	if(serResult.flag_failed == 1){ // if search failed, insert
		ptr_t tempPtr =Insert(Master2Mem, Master2SysAlloc, stackPtr, rootPtr, key);
	}
	return rootPtr;
}

/* Update */
ptr_t UpdateNode(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t stackPtr, ptr_t rootPtr, int oldKey, int newKey){
	rootPtr = DeleteTreeNode(Master2Mem, Master2SysAlloc, stackPtr, rootPtr, oldKey);
	ptr_t newPtr = Insert(Master2Mem, Master2SysAlloc, stackPtr, rootPtr, newKey);	
	return rootPtr;
}

/* Insert Node*/
ptr_t Insert(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t stackPtr, ptr_t treePtr, int data){
	int flag_stackIsUsed = 0;
	int flag_stop = 0;
	struct sub_t subResult;
	struct stack_t stackOutput;
	ptr_t currentPtr = treePtr;
	ptr_t returnPtr;
	
	while(flag_stop == 0){
		subResult = InsertSub(Master2Mem, Master2SysAlloc, currentPtr, data);
		if(subResult.feedback == FB_DONE){
			flag_stop = 1;	
			returnPtr = subResult.pointer;
			if(flag_stackIsUsed != 0){ 				
				stackOutput = dynamicStack(Master2Mem, Master2SysAlloc, stackPtr, READ_STACK, 0, 0);
				stackPtr = stackOutput.hdPtr;				
				if(subResult.flag_same_key == 0){
					// if same key doesn't exist, new node is inserted, hence the parrent node's offset needs to be updated
					if(stackOutput.operation == GOING_LEFT){
						tree_node_set_left(Master2Mem, stackOutput.pointer, subResult.pointer);
					}else{
						tree_node_set_right(Master2Mem, stackOutput.pointer, subResult.pointer);
					}
				}
				// read stack to clear stack
				while(stackPtr != NULL_PTR){
					stackOutput = dynamicStack(Master2Mem, Master2SysAlloc, stackPtr, READ_STACK, 0, 0);
					stackPtr = stackOutput.hdPtr;
				}				
			}
		}else{
			flag_stackIsUsed = 1;			
			if(subResult.feedback == FB_LEFT){				
				stackOutput = dynamicStack(Master2Mem, Master2SysAlloc, stackPtr, WRITE_STACK, currentPtr, GOING_LEFT);	
				stackPtr = stackOutput.hdPtr;
				currentPtr = tree_node_get_left_pointer(Master2Mem, currentPtr);

			}else{				
				stackOutput = dynamicStack(Master2Mem, Master2SysAlloc, stackPtr, WRITE_STACK, currentPtr, GOING_RIGHT);	
				stackPtr = stackOutput.hdPtr;
				currentPtr = tree_node_get_right_pointer(Master2Mem, currentPtr);
			}			
		}
	}
	return returnPtr;
}
struct sub_t InsertSub(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t treePtr, data_t data){
	data_t nodeData = tree_node_read_data(Master2Mem, treePtr);
	struct sub_t output;
	output.flag_same_key = 0;
	if(treePtr == NULL_PTR){
		output.pointer = tree_node_alloc_new(Master2Mem, Master2SysAlloc, data, NULL_PTR, NULL_PTR);
		output.feedback = FB_DONE;
		//return:data_inserted & pointer of new node created
	}else if(data < nodeData){
		output.feedback = FB_LEFT;
		// return:go to the left node
	}else if(data > nodeData){
		output.feedback = FB_RIGHT;
		// return:go to the right node
	}else if(data == nodeData){
		// same key already exists, don't insert
		output.pointer = treePtr;
		output.feedback = FB_DONE;
		output.flag_same_key = 1;
	}
	return output;	
}

/* Search */
struct search_t Search(data_t *Master2Mem, ptr_t treePtr, int data){
	ptr_t localPtr = treePtr;
	struct sub_t subResult;
	struct search_t output;
	int flag_found = 0;	
	
	output.flag_failed = 0;
	output.direction = 7;
	subResult = SearchSub(Master2Mem, localPtr, data);
	while(subResult.pointer != NULL_PTR && flag_found == 0){	
		if(subResult.feedback == FB_DONE){
			output.nodePtr = subResult.pointer;
			flag_found = 1;
		}else{
			output.parentPtr = localPtr;
			if(subResult.feedback == FB_LEFT){				
				localPtr = tree_node_get_left_pointer(Master2Mem, localPtr);
				output.direction = GOING_LEFT;			
			}else{
				localPtr = tree_node_get_right_pointer(Master2Mem, localPtr);
				output.direction = GOING_RIGHT;
			}		
			
			if(localPtr == NULL_PTR){
				output.flag_failed = 1;
				flag_found = 1;
			}else{
				subResult = SearchSub(Master2Mem, localPtr, data);
			}			
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

/* Delete Node */
ptr_t DeleteTreeNode(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t stackPtr, ptr_t rootPtr, int key){
	struct search_t node2delete = Search(Master2Mem, rootPtr, key);
	if (node2delete.flag_failed != 1){
		ptr_t nodePtr = node2delete.nodePtr;
		ptr_t leftPtr = tree_node_get_left_pointer(Master2Mem, nodePtr);
		ptr_t rightPtr = tree_node_get_right_pointer(Master2Mem, nodePtr);
		struct tree_ode2move;
		ptr_t tempPtr;
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
					tempPtr = Insert(Master2Mem, Master2SysAlloc, stackPtr, rootPtr,rightNodeData);				
					tree_node_write_data(Master2Mem, tempPtr, rightNodeData);
					ptr_t right_leftPtr = tree_node_get_left_pointer(Master2Mem, rightPtr);
					tree_node_set_left(Master2Mem, tempPtr, right_leftPtr);
					ptr_t right_rightPtr = tree_node_get_right_pointer(Master2Mem, rightPtr);
					tree_node_set_right(Master2Mem, tempPtr, right_rightPtr);
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
	}
	return rootPtr;
}

/* Delete Tree */
ptr_t DeleteTree(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t stackPtr, ptr_t treePtr){
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
				if(stackPtr == NULL_PTR){
					flag_stop = 1;
					returnPtr = subResult.pointer;
				}else{
					
					stackOutput = dynamicStack(Master2Mem, Master2SysAlloc, stackPtr, READ_STACK, 0, 0);
					stackPtr = stackOutput.hdPtr;
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
				stackOutput = dynamicStack(Master2Mem, Master2SysAlloc,stackPtr, WRITE_STACK, currentPtr, GOING_LEFT);	
				stackPtr = stackOutput.hdPtr;
				currentPtr = subResult.pointer;
			}else{				
				stackOutput = dynamicStack(Master2Mem, Master2SysAlloc,stackPtr, WRITE_STACK, currentPtr, GOING_RIGHT);	
				stackPtr = stackOutput.hdPtr;
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
