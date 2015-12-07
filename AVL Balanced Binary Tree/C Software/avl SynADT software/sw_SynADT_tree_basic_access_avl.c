#include "sw_SynADT.h"

/* Allocate New Node */
int *tree_node_alloc_new(data_t data){
	int *newNodePtr;
	newNodePtr = SysMalloc(sizeof(struct node_t));
	next_t offset = NULL_PTR - (int)newNodePtr;
	struct node_t newNode;
	newNode.data = data;
	newNode.left = offset;
	newNode.right = offset;
	newNode.height = 0; // should it be 0 or 1?
	tree_node_write(newNodePtr, newNode);
	return newNodePtr;
}

/* Set Pointers */
void tree_set_left_pointer(int *nowNodePtr, int *nextNodePtr){
	next_t offset;
	if(nextNodePtr == NULL){
		offset = NULL_PTR - (int)nowNodePtr;
	}else{
		offset = nextNodePtr - nowNodePtr;
	}	
	tree_node_write_left(nowNodePtr, offset);
}
void tree_set_right_pointer(int *nowNodePtr, int *nextNodePtr){
	next_t offset;
	if(nextNodePtr == NULL){
		offset = NULL_PTR - (int)nowNodePtr;
	}else{
		offset = nextNodePtr - nowNodePtr;
	}
	tree_node_write_right(nowNodePtr, offset);	
}

/* Get Pointers*/
int *tree_get_left_pointer(int *tree){
	if(tree != NULL){
		int *returnPtr;
		int pointerValue;
		next_t offset = tree_node_read_left(tree);

		pointerValue = (int)tree + offset;

		if(pointerValue == NULL_PTR){
			returnPtr = NULL;
		}else{
			returnPtr = tree + offset;
		}
		return returnPtr;
	}else{
		return NULL;
	}
}
int *tree_get_right_pointer(int *tree){
	if(tree!=NULL){
		int *returnPtr;
		int pointerValue;
		next_t offset = tree_node_read_right(tree);
		
		pointerValue = (int)tree + offset;
		if(pointerValue == NULL_PTR){
			returnPtr = NULL;
		}else{
			returnPtr = tree + offset;
		}
		return returnPtr;
		
	}else{
		return NULL;
	}
}

/* Standard Whole Node Accesses (with pointers instead of offsets) */
void tree_node_write_std(int *nodePtr, struct node_t_std nowNode){
	tree_set_left_pointer(nodePtr, nowNode.leftPtr);
	tree_set_right_pointer(nodePtr, nowNode.rightPtr);	
	tree_node_write_data(nodePtr, nowNode.data);
	tree_node_write_height(nodePtr, nowNode.height);
}
struct node_t_std tree_node_read_std(int *nodePtr){
	struct node_t_std nowNode;
	nowNode.leftPtr = tree_get_left_pointer(nodePtr);
	nowNode.rightPtr= tree_get_right_pointer(nodePtr);
	nowNode.data = tree_node_read_data(nodePtr);
	nowNode.height = tree_node_read_height(nodePtr);
	return nowNode;
}

/* Whole-node Accesses */
void tree_node_write(int *nodePtr, struct node_t nowNode){
	tree_node_write_left(nodePtr, nowNode.left);
	tree_node_write_right(nodePtr, nowNode.right);	
	tree_node_write_data(nodePtr, nowNode.data);
	tree_node_write_height(nodePtr, nowNode.height);
}
struct node_t tree_node_read(int *nodePtr){
	struct node_t nowNode;
	nowNode.left = tree_node_read_left(nodePtr);
	nowNode.right= tree_node_read_right(nodePtr);
	nowNode.data = tree_node_read_data(nodePtr);
	nowNode.height = tree_node_read_height(nodePtr);
	return nowNode;
}

/* Memory Accesses */
/* Writes */
void tree_node_write_height(int *nodePtr, int height){
	nodePtr[HEIGHT_OFFSET] = height;
}
void tree_node_write_left(int *nodePtr, next_t offset){
	nodePtr[LEFT_OFFSET] = offset;
}
void tree_node_write_right(int *nodePtr, next_t offset){
	nodePtr[RIGHT_OFFSET] = offset;
}
void tree_node_write_data(int *nodePtr, data_t data){
	nodePtr[DATA_OFFSET] = data;
}
/* Reads */
int tree_node_read_height(int *nodePtr){
	return nodePtr[HEIGHT_OFFSET];
}
next_t tree_node_read_left(int *nodePtr){
	return nodePtr[LEFT_OFFSET];
}
next_t tree_node_read_right(int *nodePtr){
	return nodePtr[RIGHT_OFFSET];
}
data_t tree_node_read_data(int *nodePtr){
	return nodePtr[DATA_OFFSET];
}