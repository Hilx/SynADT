#include <stdio.h>
#include <stdlib.h>

#define NULL_PTR 0

struct node_t{
	unsigned left; //0
	unsigned right;	//1
	int data; //2
};

struct insert_t{
	int *pointer;
	int flag;
};

struct stack_t{
	int index;
	int *pointer;
	int operation;
};

/* Stack Access Commands */
#define READ_STACK 0
#define WRITE_STACK 1

#define FLAG_READ_STACK 1
#define FLAG_GO_LEFT 2
#define FLAG_GO_RIGHT 3

static int stackSize = 20;

/* access */
int *get_left_pointer(int *tree);
int *get_right_pointer(int *tree);
int get_node_data(int *tree);
void write_left_pointer(int *tree, int *nextPtr);
void write_right_pointer(int *tree, int *nextPtr);

/* manipulations */
int *TreeGen(int NumberOfNodes, int *root, int *pointerStack, int*operationStack);
int *Insert(int data, int *root, int *pointerStack, int *operationStack);
struct insert_t	Insert_subfunc(int data, int *tree);
struct stack_t AccessStack(int index, int command, int *pointer, int operation, int *pointerStack, int *operationStack);


int *Search(int data, int *tree);
int *DeleteTree(int *tree);


/* software only */
void printTree(int *tree);
void printValue(int *tree);

int main(){
	int *root = NULL;
	int NumberOfNodes = 2;	
	int searchKey = 3;	
	int *foundPtr;	
	int i;
	/* STACK */	
	int *pointerStack, *operationStack;
	pointerStack = (int*)malloc(sizeof(int)*stackSize);
	operationStack = (int*)malloc(sizeof(int)*stackSize);
	for(i = 0;i < stackSize; i++){
		operationStack[i] = 7;
	}

	/* Create the tree */
	root = TreeGen(NumberOfNodes, root, pointerStack, operationStack); 	
	printTree(root);
	
	/* Search */	
	//foundPtr = Search(searchKey, root);
	//printf("Search for [ %d ] and found [%d]\n",searchKey,foundPtr[2]);	
	
	/* Delete the three */
	//root = DeleteTree(root);	
	//printf("print list after deletion:\n");
	//printTree(root);
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
int *TreeGen(int NumberOfNodes, int *root, int *pointerStack, int*operationStack){
	int i;
	printf("before first insertion, root points at %d\n",i,root);
	for(i = 0; i <NumberOfNodes; i++){
		printf("*** Insertion %d: ***\n",i);
		root = Insert(i, root, pointerStack, operationStack);
		printf("Current root %d\n\n",root);
	}
	return root;	
}
int *Insert(int data, int *root, int *pointerStack, int *operationStack){
	int *tree = root;
	struct insert_t interResult;
	struct stack_t stackOut;
	int stackIndex;
	stackIndex = 0;
	int FLAG_INSERT_STOP;
	FLAG_INSERT_STOP = 0;
	while(FLAG_INSERT_STOP == 0){
		interResult = Insert_subfunc(data, tree);
		printf("flag %d\n",interResult.flag);
		if(interResult.flag == FLAG_READ_STACK){ //[IN NEXT ITERATION]Try to insert in the current node
			printf("index is %d\n",stackIndex);
			// read the parent node from stack
			// AccessStack(int index, int command, int *pointer, int operation, int *pointerStack, *int operationStack);
			if(stackIndex - 1 >= 0){
				printf("About to read stack\n");
				stackOut = AccessStack(stackIndex, READ_STACK, NULL, 0, pointerStack, operationStack);
				stackIndex = stackOut.index;
				printf("thats done\n");
				tree = stackOut.pointer;
				if(stackOut.operation == FLAG_GO_LEFT){
					write_left_pointer(tree, interResult.pointer);
				}else if(stackOut.operation == FLAG_GO_RIGHT){
					write_right_pointer(tree, interResult.pointer);				
				}
			}else{
				FLAG_INSERT_STOP = 1;
				tree = interResult.pointer;
				operationStack[0] = 7;
			}
		}else if(interResult.flag == FLAG_GO_LEFT || interResult.flag == FLAG_GO_RIGHT){ //[WRITE STACK]
			//[IN NEXT ITERATION]go into node's left/right next
			// write current node's location to stack
			// AccessStack(int index, int command, int *pointer);
			printf("Go down\n");
			printf("tree pointer value %d\n",tree);
			stackOut = AccessStack(stackIndex, WRITE_STACK, tree, interResult.flag, pointerStack, operationStack);		
			stackIndex = stackOut.index;
			tree = interResult.pointer;
		}
	}
	return tree;
}



/* Dedicated Stack */
// AccessStack(int index, int command, int *pointer, int operation, int *pointerStack, *int operationStack);
struct stack_t AccessStack(int index, int command, int *pointer, int operation, int *pointerStack, int *operationStack){
	struct stack_t output;	
	int internal_index;
	printf("Calling AccessStack(), command = [%d], \n", command);
	if(command == READ_STACK){		
		printf("reading stack\n");
		printf("READING POINTER %d\n",pointerStack[index]);
		output.pointer = (int*)pointerStack[index];
		printf("reading stack 2\n");
		output.operation = operationStack[index];
		output.index = index - 1;
		printf("in stack func, index = %d\n",index);
	}else if (command == WRITE_STACK){
	
		if(index == 0 && operationStack[index] == 7){ 
			internal_index = index;
		}else{
			internal_index = index + 1;
		}
		printf("WRITING POINTER VALUE %d TO STACK[%d]\n", pointer,internal_index);
		pointerStack[internal_index] = (int)pointer;
		operationStack[internal_index] = operation;
		printf("------------------------------------reading from stack %d\n",pointerStack[internal_index]);
		output.index = internal_index;
	}	
	return output;	
}

struct insert_t	Insert_subfunc(int data, int *tree){
	struct insert_t returnValue;
	int *newNode;

	if(tree == NULL){
		newNode = (int*)malloc(sizeof(struct node_t));
		newNode[2] = data;
		newNode[0] = NULL_PTR - (int)newNode;
		newNode[1] = NULL_PTR - (int)newNode;	
		returnValue.pointer = newNode;
		returnValue.flag = FLAG_READ_STACK;
	}else if(data < get_node_data(tree)){ //case2
		newNode = get_left_pointer(tree);
		returnValue.pointer = newNode;
		returnValue.flag = FLAG_GO_LEFT;
	}else{ //case3
		newNode = get_right_pointer(tree);
		returnValue.pointer = newNode;
		returnValue.flag = FLAG_GO_RIGHT;
	}	

	return returnValue;		
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
}
