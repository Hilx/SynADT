#include "main.h"
/*
int main(){
	int *myHeap = (int*)malloc(sizeof(int) * HEAP_SIZE);
	
	ptr_t hdPtr = NULL_PTR;
	int SizeOfList = 10;
	
	hdPtr = ListGen(myHeap, SizeOfList ,hdPtr);	
	PrintList(myHeap, hdPtr);
	
	hdPtr = DeleteListNode_HT( myHeap, hdPtr, 5);
	PrintList(myHeap, hdPtr);	
	hdPtr = DeleteListNode_HT( myHeap, hdPtr, 0);
	PrintList(myHeap, hdPtr);
	hdPtr = DeleteListNode_HT( myHeap, hdPtr, 9);
	PrintList(myHeap, hdPtr);
	
	ptr_t tempPtr;
	tempPtr = SearchInList(myHeap, hdPtr, 7);
	printf("node found with data = %d\n", list_node_read_data(myHeap,tempPtr));

	return 0;
}
*/

/* Creating a list */
ptr_t ListGen(int *myHeap,int SizeOfList, ptr_t hdPtr){
	int i;
	int input[10] = {5,3,9,4,1,2,8,7,0,6};
	for(i = 0; i< SizeOfList;i++){
		hdPtr = Add2List(myHeap, input[i], hdPtr);
	}
	
	return hdPtr;
}

ptr_t Add2List(int *myHeap, int data, ptr_t hdPtr){	
	ptr_t insertPtr;
	ptr_t currentPtr, nextPtr;
	int currentKey, nextKey;
	int nextPtr_value;
	
	// search for insertion point
	if(hdPtr == NULL_PTR){
		insertPtr = hdPtr;
	}else{
		// search for appropriate point to insert the new node
		currentPtr = hdPtr;
		nextPtr = list_node_get_next_pointer(myHeap, currentPtr);
		currentKey = list_node_read_data(myHeap, currentPtr);
		nextKey = list_node_read_data(myHeap, nextPtr);
		// while not the end of list && data
		if(data > currentKey){
			while(nextPtr != NULL_PTR && data > nextKey){
				currentPtr = nextPtr;				
				nextPtr = list_node_get_next_pointer(myHeap, currentPtr);
				nextKey = list_node_read_data(myHeap, nextPtr);				
			}
			insertPtr = currentPtr;			
		}
	}

	// insert
	ptr_t newNode;
	if(data > currentKey){
		if(insertPtr == NULL_PTR){
			newNode = list_node_alloc_new(myHeap, data, NULL_PTR);
			hdPtr = newNode;
		}else{
			newNode = list_node_alloc_new(myHeap, data, nextPtr);
			list_node_set_next(myHeap, insertPtr, newNode);
			hdPtr = hdPtr;
		}	
	}else{
		newNode = list_node_alloc_new(myHeap, data, hdPtr);		
		hdPtr = newNode;
	}
	return hdPtr;
}


ptr_t DeleteListNode_HT(int *myHeap, ptr_t hdPtr, int key){	
	ptr_t currentPtr = hdPtr;
	int currentKey = list_node_read_data(myHeap, currentPtr);
	ptr_t nextPtr, savePtr;
	int flag_search = 0;
	
	// search
	while(currentKey != key && currentPtr != NULL_PTR){
		flag_search = 1;
		nextPtr = list_node_get_next_pointer(myHeap, currentPtr);
		
		savePtr = currentPtr;
		currentPtr = nextPtr;
		currentKey = list_node_read_data(myHeap, currentPtr);
	}
	
	// delete	
	if(flag_search == 0){
		nextPtr = list_node_get_next_pointer(myHeap, currentPtr);
		hdPtr = nextPtr;
		list_node_delete(myHeap, currentPtr);		
	}else{
		nextPtr = list_node_get_next_pointer(myHeap, currentPtr);	
		list_node_set_next(myHeap, savePtr, nextPtr);
		list_node_delete(myHeap, currentPtr);
		hdPtr = hdPtr;
	}
	
	return hdPtr;	
}

ptr_t SearchInList(int *myHeap, ptr_t hdPtr, int key){
	ptr_t currentPtr = hdPtr;
	int currentKey = list_node_read_data(myHeap, currentPtr);
	int nextPtr;
	while(currentKey != key && currentPtr != NULL_PTR){
		nextPtr = list_node_get_next_pointer(myHeap, currentPtr);
		currentPtr = nextPtr;
		currentKey = list_node_read_data(myHeap, currentPtr);
	}
	
	int outputPtr;
	if(currentPtr == NULL_PTR){
		outputPtr = SEARCH_NOT_FOUND;
	}else{
		outputPtr = currentPtr;
	}
	return outputPtr;
}


/* SOFTWARE ONLY */
/* Print the list*/
void PrintList(int *myHeap, ptr_t hdPtr){	
	ptr_t nowPtr;
	int nextPtr_value;
	int Flag_Stop;
	ptr_t nextPtr;
	
	nowPtr = hdPtr;
	Flag_Stop = 0;
	if (nowPtr != NULL_PTR){ //nowPtr !=0
		printf("Printing List [");
		while(Flag_Stop == 0){ //nowPtr != 0
			printf(" %d", list_node_read_data(myHeap, nowPtr));	
			nextPtr = list_node_get_next_pointer(myHeap, nowPtr);
			if(nextPtr == NULL_PTR){
				Flag_Stop = 1;
			}else{
				nowPtr = nextPtr;
			}
			
		}	
		printf("]\n");	
	}else{
		printf("The list is empty.\n");
	}
}

/*   */
// write
void list_node_write_next(int *myHeap,ptr_t nodePtr, signed next){
	myHeap[nodePtr + 1] = next;
}
void list_node_write_data(int *myHeap,ptr_t nodePtr, int key){
	myHeap[nodePtr + 0] = key;
}
void list_node_write(int *myHeap, ptr_t nodePtr, struct list_node_t currentNode){
	list_node_write_next(myHeap, nodePtr, currentNode.next);
	list_node_write_data(myHeap, nodePtr, currentNode.data);
}
// read
int list_node_read_next(int *myHeap, ptr_t nodePtr){
	return myHeap[nodePtr +1];
}
int list_node_read_data(int *myHeap, ptr_t nodePtr){
	return myHeap[nodePtr +0];
}
struct list_node_t list_node_read(int *myHeap, ptr_t nodePtr){
	struct list_node_t node;
	node.next = list_node_read_next(myHeap, nodePtr);
	node.data = list_node_read_data(myHeap, nodePtr);
	return node;
}
// others
void list_node_set_next(int *myHeap, ptr_t currentNodePtr, ptr_t nextNodePtr){
	list_node_write_next(myHeap, currentNodePtr, nextNodePtr - currentNodePtr);
}
ptr_t list_node_alloc_new(int *myHeap, int data, ptr_t nextNodePtr){
	ptr_t newNodePtr = SysMalloc(WORDS_PER_NODE);
	struct list_node_t newNode;
	newNode.data = data;
	newNode.next = nextNodePtr - newNodePtr;
	list_node_write(myHeap, newNodePtr, newNode);
	return newNodePtr;
}
ptr_t list_node_get_next_pointer(int *myHeap, ptr_t currentNodePtr){
	next_t offset = list_node_read_next(myHeap, currentNodePtr);
	return currentNodePtr + offset;
}
void list_node_delete(int *myHeap, ptr_t nodePtr){
	SysFree(nodePtr);
}
