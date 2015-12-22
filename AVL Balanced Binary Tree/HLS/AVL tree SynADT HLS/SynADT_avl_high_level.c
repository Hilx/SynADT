#include "SynADT.h"

/* Application Level */
/* Check then Insert */
ptr_t Check_thenInsert(data_t *myHeap, data_t *Master2SysAlloc, data_t *fixedStack, ptr_t stackPtr_avl, ptr_t rootPtr, int key){	
	struct search_t serResult = Search(myHeap, fixedStack, stackPtr_avl, rootPtr, key);	
	if(serResult.flag_failed == 1){ // if search failed, insert
		struct ptrBundle_t newBundle = Insert(myHeap, Master2SysAlloc, fixedStack, stackPtr_avl, rootPtr, key, NULL_PTR, NULL_PTR);
		rootPtr = newBundle.root;
	}
	return rootPtr;
}
/* update */
ptr_t UpdateNode(data_t *myHeap, data_t *Master2SysAlloc, data_t *fixedStack, data_t *secondStack, ptr_t stackPtr_avl, ptr_t rootPtr, int oldKey, int newKey){
	rootPtr = DeleteTreeNode(myHeap, Master2SysAlloc, fixedStack,secondStack, stackPtr_avl, rootPtr, oldKey);
	struct ptrBundle_t newBundle = Insert(myHeap, Master2SysAlloc, fixedStack, stackPtr_avl, rootPtr, newKey, NULL_PTR, NULL_PTR);
	return newBundle.root;
}

struct ptrBundle_t Insert(data_t *myHeap, data_t *Master2SysAlloc, data_t *fixedStack, ptr_t stackPtr_avl, ptr_t treePtr, int data, ptr_t new_leftPtr, ptr_t new_rightPtr){

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
		subResult = InsertSub(myHeap, Master2SysAlloc, currentPtr, data, new_leftPtr, new_rightPtr);
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
					while(stackPtr_avl > 0){
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
				currentPtr = node_get_left_pointer(myHeap, currentPtr);
			}else{
				stackOutput = avlStack(fixedStack, stackPtr_avl, WRITE_STACK, currentPtr, GOING_RIGHT);
				stackPtr_avl = stackOutput.hdPtr_avl;
				currentPtr = node_get_right_pointer(myHeap, currentPtr);
			}				
		}
	}
	return output;
}

struct sub_t InsertSub(data_t *myHeap, data_t *Master2SysAlloc, ptr_t treePtr, int data , ptr_t new_leftPtr, ptr_t new_rightPtr){

	data_t nodeData = node_read_data(myHeap, treePtr);		
	struct sub_t output;
	output.flag_same_key = 0;
	if(treePtr == NULL_PTR){
		ptr_t tempPtr = node_alloc_new(myHeap, Master2SysAlloc, data, new_leftPtr, new_rightPtr);
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
/* Search */
struct search_t Search(data_t *myHeap, data_t *fixedStack, ptr_t stackPtr_avl, ptr_t treePtr, int data){

	ptr_t localPtr = treePtr;
	struct sub_t subResult;
	struct search_t output;
	int flag_found = 0;	
	struct stack_t stackOutput;
	int flag_stackIsUsed = 0;
	
	output.flag_failed = 0;
	output.direction = 7;
	subResult = SearchSub(myHeap, localPtr, data);
	//while(subResult.pointer != NULL_PTR && flag_found == 0){
	while(localPtr != NULL_PTR && flag_found == 0){
		if(subResult.feedback == FB_DONE){
			output.nodePtr = subResult.pointer;
			flag_found = 1;
			if(flag_stackIsUsed ==  1){
				output.stackPtr_avl = stackPtr_avl;
			}else{
				output.stackPtr_avl = 0;
			}
		}else{
			flag_stackIsUsed = 1;
			
			output.parentPtr = localPtr;
			if(subResult.feedback == FB_LEFT){
				stackOutput = avlStack(fixedStack, stackPtr_avl, WRITE_STACK, localPtr, GOING_LEFT);
				stackPtr_avl = stackOutput.hdPtr_avl;
				localPtr = node_get_left_pointer(myHeap, localPtr);
			}else{
				stackOutput = avlStack(fixedStack, stackPtr_avl, WRITE_STACK, localPtr, GOING_RIGHT);
				stackPtr_avl = stackOutput.hdPtr_avl;
				localPtr = node_get_right_pointer(myHeap, localPtr);
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

struct sub_t SearchSub(data_t *myHeap, ptr_t treePtr, int data){

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
/* Delete Tree */
ptr_t DeleteTree(data_t *myHeap, data_t *Master2SysAlloc, data_t *fixedStack, ptr_t stackPtr_avl, ptr_t treePtr){

	int flag_stackIsUsed = 0;
	int flag_stop = 0;
	struct sub_t subResult;
	struct stack_t stackOutput;
	ptr_t currentPtr = treePtr;
	ptr_t returnPtr;	
	
	while(flag_stop == 0){
		
		subResult = DeleteTreeSub(myHeap, Master2SysAlloc, currentPtr);
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
struct sub_t DeleteTreeSub(data_t *myHeap, data_t *Master2SysAlloc, ptr_t treePtr){

	struct sub_t output;
	ptr_t leftPtr = node_get_left_pointer(myHeap, treePtr);
	ptr_t rightPtr = node_get_right_pointer(myHeap, treePtr);
	if(treePtr == NULL_PTR){
		output.pointer = NULL_PTR;
		output.feedback = FB_DONE;
	}else if(leftPtr == NULL_PTR && rightPtr == NULL_PTR){
		node_delete(Master2SysAlloc, treePtr);
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


/* --------------------------------------------------------------------------------------------------------- */
/* Delete Node */
ptr_t DeleteTreeNode(data_t *myHeap, data_t *Master2SysAlloc, data_t *fixedStack,  data_t *secondStack, int stackPtr_avl, ptr_t rootPtr, int key){
	int flag_stop = 0;
	int flag_search_failed = 0;
	int flag_stackIsUsed = 0;
	ptr_t nowPtr = rootPtr;
	struct stack_t stackOutput;
	while(flag_stop == 0){
		struct sub_t subResult = DeleteNodeSub(myHeap, nowPtr, key);
		if(subResult.feedback == FB_DONE){
			flag_stop = 1;
			// --------- Deletion ---------
			ptr_t leftPtr = node_get_left_pointer(myHeap, nowPtr);
			ptr_t rightPtr = node_get_right_pointer(myHeap, nowPtr);
			if(leftPtr == NULL_PTR || rightPtr == NULL_PTR){ // at least one null children
				if(leftPtr != NULL_PTR){
					struct node_t_std nowNode = node_read_std(myHeap, leftPtr);
					node_write_std(myHeap, nowPtr, nowNode);
					node_delete(Master2SysAlloc,leftPtr);
				}else if(rightPtr != NULL_PTR){
					struct node_t_std nowNode = node_read_std(myHeap, rightPtr);
					node_write_std(myHeap, nowPtr, nowNode);
					node_delete(Master2SysAlloc,rightPtr);					
				}else{
					node_delete(Master2SysAlloc,nowPtr);
					nowPtr = NULL_PTR;
				}				
			}else{ // both leaves exists
				//1.get the inorder successor
				ptr_t rightPtr = node_get_right_pointer(myHeap, nowPtr);
				ptr_t minPtr = minValueNode(myHeap, rightPtr);

				//2.copy the inorder successor's data to this node
				int copyData = node_read_data(myHeap, minPtr);
				node_write_data(myHeap, nowPtr, copyData);				
				//3.delete the inorder successor
				ptr_t tempPtr = DeleteSuccessor(myHeap, Master2SysAlloc, secondStack, 0, rightPtr, copyData);
				node_set_right(myHeap, nowPtr, tempPtr);				
			}			
			// --------- Balancing ---------
			if(flag_stackIsUsed == 0){
				
				if(nowPtr == NULL_PTR){
					rootPtr = nowPtr;
				}else{
					rootPtr = ProcessNodeDeletion(myHeap,nowPtr);
				}
			}else{
				
				stackOutput = avlStack(fixedStack, stackPtr_avl, READ_STACK, 0, 0);		
				stackPtr_avl = stackOutput.hdPtr_avl;		
				if(stackOutput.operation == GOING_LEFT){
					node_set_left(myHeap, stackOutput.pointer, nowPtr);
				}else{
					node_set_right(myHeap, stackOutput.pointer, nowPtr);
				}
				while(stackPtr_avl > 0){
					nowPtr = stackOutput.pointer;
					ptr_t nowPtr_new = ProcessNodeDeletion(myHeap, nowPtr);		
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
				rootPtr = ProcessNodeDeletion(myHeap, stackOutput.pointer);					
			}			
		}else{
			flag_stackIsUsed = 1;
			if(subResult.feedback == FB_LEFT){
				stackOutput = avlStack(fixedStack, stackPtr_avl, WRITE_STACK, nowPtr, GOING_LEFT);
				stackPtr_avl = stackOutput.hdPtr_avl;
				nowPtr = node_get_left_pointer(myHeap, nowPtr);
			}else{
				stackOutput = avlStack(fixedStack, stackPtr_avl, WRITE_STACK, nowPtr, GOING_RIGHT);
				stackPtr_avl = stackOutput.hdPtr_avl;
				nowPtr = node_get_right_pointer(myHeap, nowPtr);
			}			
			if(nowPtr == NULL_PTR){
				flag_stop = 1;
				flag_search_failed = 1;
			}			
		}		
	}
	return rootPtr;
}
struct sub_t DeleteNodeSub(data_t *myHeap, ptr_t nodePtr, int key){
	int nowKey = node_read_data(myHeap, nodePtr);
	struct sub_t out;
	if(key == nowKey){
		out.feedback = FB_DONE;
	}else if(key < nowKey){
		out.feedback = FB_LEFT;
	}else if(key > nowKey){
		out.feedback = FB_RIGHT;
	}
	return out;
}

ptr_t minValueNode(data_t *myHeap, ptr_t nodePtr){
	ptr_t currentPtr = nodePtr;
	ptr_t leftPtr = node_get_left_pointer(myHeap, currentPtr);
	while(leftPtr != NULL_PTR){
		currentPtr = leftPtr;
		leftPtr = node_get_left_pointer(myHeap, currentPtr);
	} 
	return currentPtr;
}

// --- delete the inorder successor
/* Delete Node */
ptr_t DeleteSuccessor(data_t *myHeap, data_t *Master2SysAlloc, data_t *fixedStack, int stackPtr_avl, ptr_t rootPtr, int key){
	int flag_stop = 0;
	int flag_search_failed = 0;
	ptr_t nowPtr = rootPtr;
	ptr_t outPtr;
	struct stack_t stackOutput;
	int flag_stackIsUsed = 0;
	while(flag_stop == 0){
		struct sub_t subResult = DeleteSuccessorSub(myHeap, nowPtr, key);
		if(subResult.feedback == FB_DONE){
			flag_stop = 1;
			// --------- Deletion ---------
			node_delete(Master2SysAlloc, nowPtr);
			nowPtr = NULL_PTR;		
			// --------- Balancing ---------
			if(flag_stackIsUsed == 0){
				outPtr = NULL_PTR;
			}else{
				stackOutput = avlStack(fixedStack, stackPtr_avl, READ_STACK, 0, 0);		
				stackPtr_avl = stackOutput.hdPtr_avl;		
				if(stackOutput.operation == GOING_LEFT){
					node_set_left(myHeap, stackOutput.pointer, nowPtr);
				}else{
					node_set_right(myHeap, stackOutput.pointer, nowPtr);
				}
				while(stackPtr_avl > 0){
					nowPtr = stackOutput.pointer;
					ptr_t nowPtr_new = ProcessNodeDeletion(myHeap, nowPtr);		
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
				outPtr = ProcessNodeDeletion(myHeap, stackOutput.pointer);
			}
		}else{
			flag_stackIsUsed = 1;
			if(subResult.feedback == FB_LEFT){
				stackOutput = avlStack(fixedStack, stackPtr_avl, WRITE_STACK, nowPtr, GOING_LEFT);
				stackPtr_avl = stackOutput.hdPtr_avl;
				nowPtr = node_get_left_pointer(myHeap, nowPtr);
			}else{
				stackOutput = avlStack(fixedStack, stackPtr_avl, WRITE_STACK, nowPtr, GOING_RIGHT);
				stackPtr_avl = stackOutput.hdPtr_avl;
				nowPtr = node_get_right_pointer(myHeap, nowPtr);
			}
		}		
	}
	return outPtr;
}
struct sub_t DeleteSuccessorSub(data_t *myHeap, ptr_t nodePtr, int key){
	int nowKey = node_read_data(myHeap, nodePtr);
	struct sub_t out;
	if(key == nowKey){
		out.feedback = FB_DONE;
	}else if(key < nowKey){
		out.feedback = FB_LEFT;
	}else if(key > nowKey){
		out.feedback = FB_RIGHT;
	}
	return out;
}
