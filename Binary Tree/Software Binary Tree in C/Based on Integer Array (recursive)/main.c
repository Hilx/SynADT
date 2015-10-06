#include <stdio.h>
#include <stdlib.h>

#define NULL_PTR 0

struct node_t{
	unsigned left; //0
	unsigned right;	//1
	int data; //2
};

/* access */
int *get_left_pointer(int *tree);
int *get_right_pointer(int *tree);
int get_node_data(int *tree);
void write_left_pointer(int *tree, int *nextPtr);
void write_right_pointer(int *tree, int *nextPtr);

/* manipulations */
int *Insert(int data, int *tree);
int *TreeGen(int NumberOfNodes, int *root);
int *Search(int data, int *tree);
int *DeleteTree(int *tree);

/* software only */
void printTree(int *tree);
void printValue(int *tree);

int main(){
	int *root = NULL;
	int NumberOfNodes = 5;	
	int searchKey = 3;	
	int *foundPtr;	

	/* Create the tree */
	root = TreeGen(NumberOfNodes, root); 	
	printTree(root);
	
	/* Search */	
	foundPtr = Search(searchKey, root);
	printf("Search for [ %d ] and found [%d]\n",searchKey,foundPtr[2]);	
	
	/* Delete the three */
	root = DeleteTree(root);	
	printf("print list after deletion:\n");
	printTree(root);
}
/* access */
int *get_left_pointer(int *tree){
	int *returnPtr;
	int pointerValue;
	pointerValue = (int)tree + tree[0];
	if(pointerValue == NULL_PTR){
		returnPtr = NULL;
	}else{
		returnPtr = tree + tree[0];
	}
	return returnPtr;
}
int *get_right_pointer(int *tree){
	int *returnPtr;
	int pointerValue;
	pointerValue = (int)tree + tree[1];
	if(pointerValue == NULL_PTR){
		returnPtr = NULL;
	}else{
		returnPtr = tree + tree[1];
	}
	return returnPtr;
}
int get_node_data(int *tree){
	return tree[2];
}

void write_left_pointer(int *tree, int *nextPtr){
	unsigned offset = nextPtr - tree;
	tree[0] = offset;
}
void write_right_pointer(int *tree, int *nextPtr){
	unsigned offset = nextPtr - tree;
	tree[1] = offset;
}

/* manipulations */
int *Insert(int data, int *tree){
	int *newNode;
	//printf("*INSERTING*\n");
	if(tree == NULL){		
		//printf("[insert]\n");
		//printf("addr after this insert: %d\n",tree);
		newNode = (int*)malloc(sizeof(struct node_t));
		newNode[2] = data;
		newNode[0] = NULL_PTR - (int)newNode;
		newNode[1] = NULL_PTR - (int)newNode;
		tree = newNode;		
		//printf("[%d, %d]",newNode[0],newNode[1]);
	}else if(data < get_node_data(tree)){
		//printf("[left]\n");
		newNode = get_left_pointer(tree);
		newNode = Insert(data, newNode);
		write_left_pointer(tree, newNode);
	}else{
		//printf("[right]\n");
		newNode = get_right_pointer(tree);
		newNode = Insert(data, newNode);
		write_right_pointer(tree, newNode);		
	}	
	//printf("addr after this insert: %d\n",tree);
	return tree;
}

int *Search(int data, int *tree){
	int readData;
	int *getPtr;
	if(tree != NULL){
		readData = get_node_data(tree);
		if(data == readData){
			return tree;
		}else if(data < readData){
			getPtr = get_left_pointer(tree);
			return Search(data, getPtr); 
		}else{
			getPtr = get_right_pointer(tree);
			return Search(data, getPtr); 			
		}		
	}else{
		return NULL;
	}
}

int *DeleteTree(int *tree){
	int *leftPtr, *rightPtr;
	int *returnPtr;
	int *tempPtr;
	leftPtr = get_left_pointer(tree);
	rightPtr = get_right_pointer(tree);
	
	if(tree == NULL){
		//printf("empty \n");
		return NULL;
	}else if(leftPtr == NULL && rightPtr == NULL){
		//printf("empty \n");
		free(tree);	
		return NULL;	
	}else{
		if(leftPtr != NULL){
		//	printf("delete left \n");
			tempPtr = DeleteTree(leftPtr);
			write_left_pointer(tree, tempPtr);
		}
		if(rightPtr != NULL){
		//	printf("delete right \n");
			tempPtr = DeleteTree(rightPtr);
			write_right_pointer(tree, tempPtr);
		}
		//printf("free this one \n");
		free(tree);
		return NULL;
	}
}

int *TreeGen(int NumberOfNodes, int *root){
	int i;
	int *temp;
	for(i = 0; i <NumberOfNodes; i++){
		if(i == 0){
			root = Insert(i, root);
		}else{
			temp = Insert(i, root);
		}
	
		//printf("%dth inserted, root points at %d\n",i,root);
	}
	return root;	
}

/* software only */
void printTree(int *tree){
	printf("Printing the tree now [ ");
	printValue(tree);
	printf(" ] Done printing.\n");
}
void printValue(int *tree){
	//printf("nowNode %d\n",tree);
	if(tree != NULL){
		//printf("go left\n");
		printValue(get_left_pointer(tree));
		printf(" %d ",get_node_data(tree));
		//printf("go right\n");
		printValue(get_right_pointer(tree));
	}
	//else{
		//printf("empty node\n");
	//}
}