#include "sw_SynADT.h"

/* Check then Insert */
int *Check_thenInsert(int *root, data_t key){
	int *nodePtr = Search(key, root);
	if(nodePtr == NULL){ // if search failed, insert
		nodePtr = Insert(key, root);
	}
	return root;
}

/* Update */
int *UpdateNode(int *root, int oldKey, int newKey){	
	root = DeleteTreeNode(root, oldKey);
	int *newPtr;
	newPtr = Insert(newKey, root);	
	return root;
}

/* Insert */
int *Insert(int data, int *tree){
	int *aNodePtr, *bNodePtr;
	int flag_already_exist = 0;
	if(tree == NULL){	
		tree = tree_node_alloc_new(data);	
	}else if(data < tree_node_read_data(tree)){
		aNodePtr = tree_get_left_pointer(tree);
		bNodePtr = Insert(data, aNodePtr);
		if(bNodePtr != aNodePtr){
			tree_set_left_pointer(tree, bNodePtr);			
		}
	}else if(data > tree_node_read_data(tree)){
		aNodePtr = tree_get_right_pointer(tree);
		bNodePtr = Insert(data, aNodePtr);
		if(bNodePtr != aNodePtr){
			tree_set_right_pointer(tree, bNodePtr);		
		}
	}else if(data == tree_node_read_data(tree)){
		// don't insert
		tree = tree;
		flag_already_exist = 1;
	}
	
	// ------- Balancing -------
	if(flag_already_exist == 0){
		int *leftPtr, *rightPtr;
		leftPtr = tree_get_left_pointer(tree);
		rightPtr = tree_get_right_pointer(tree);
		
		tree_node_write_height(tree, calc_height(tree));
		signed balance = getBalance(tree);
		
		// If this node becomes unbalanced
		int *node = tree;
		
		if (balance > 1 && data < tree_node_read_data(leftPtr)){
			return rightRotate(node);
		}else if (balance < -1 && data > tree_node_read_data(rightPtr)){
			return leftRotate(node);
		}else if (balance > 1 && data > tree_node_read_data(leftPtr))
		{
			tree_set_left_pointer(node,leftRotate(leftPtr));
			return rightRotate(node);
		}else if (balance < -1 && data <tree_node_read_data(rightPtr))
		{
			tree_set_right_pointer(node,rightRotate(rightPtr));
			return leftRotate(node);
		}	
		
		tree = node;
	}
	return tree;
}

/* Search */
int *Search(int data, int *tree){
	int readData;
	int *getPtr;
	if(tree != NULL){
		readData = tree_node_read_data(tree);
		if(data == readData){
			return tree;
		}else if(data < readData){
			getPtr = tree_get_left_pointer(tree);
			return Search(data, getPtr); 
		}else{
			getPtr = tree_get_right_pointer(tree);
			return Search(data, getPtr); 			
		}		
	}else{
		return NULL;
	}
}

/* Delete Node */
int *DeleteTreeNode(int *root, int key){
	// search is done inside the deleting process
	
	if(root == NULL){
		return root;
	}
	else{
		struct node_t_std nowNode;
		nowNode = tree_node_read_std(root);		
		if(key < nowNode.data){
			if(nowNode.leftPtr != NULL){
				tree_set_left_pointer(root, DeleteTreeNode(nowNode.leftPtr,key)); 
			}
		}else if(key > nowNode.data){
			if(nowNode.rightPtr != NULL){
				tree_set_right_pointer(root, DeleteTreeNode(nowNode.rightPtr,key));
			}
		}else{
			if(nowNode.leftPtr == NULL){
				int *temp = nowNode.rightPtr;
				SysFree(root);
				return temp;
			}else if(nowNode.rightPtr == NULL){
				int *temp = nowNode.leftPtr;
				SysFree(root);
				return temp;							
			}		
			int *temp = minValueNode(nowNode.rightPtr);
			tree_node_write_data(root, tree_node_read_data(temp));			
			tree_set_right_pointer(root, DeleteTreeNode(nowNode.rightPtr, tree_node_read_data(temp)));
		}
	}
	return root;
}
int *minValueNode(int *node){
	int *current = node;
	while(tree_get_left_pointer(current) != NULL){
		current = tree_get_left_pointer(current);
	}
	return current;
}

/* Delete Tree */
int *DeleteTree(int *tree){
	int *leftPtr, *rightPtr;
	int *returnPtr;
	int *tempPtr;
	leftPtr = tree_get_left_pointer(tree);
	rightPtr = tree_get_right_pointer(tree);
	
	if(tree == NULL){
		return NULL;
	}else if(leftPtr == NULL && rightPtr == NULL){
		SysFree(tree);	
		return NULL;	
	}else{
		if(leftPtr != NULL){
			tempPtr = DeleteTree(leftPtr);
			tree_set_left_pointer(tree, tempPtr);
		}
		if(rightPtr != NULL){
			tempPtr = DeleteTree(rightPtr);
			tree_set_right_pointer(tree, tempPtr);
		}
		SysFree(tree);
		return NULL;
	}
}