#include "main.h"

int main(void){
	/* Heap, Stack */
	int *fixedStack = (int*)malloc(sizeof(int)*33);
	
	myHeapIndex = 0;
	int *myHeap = (int*)malloc(sizeof(int)*NODE_SIZE*HEAP_SIZE); 
	int *stackPtr = NULL;
	ptr_t root;
	struct search_t searchResult;
	struct sub_t subResult;
	int searchKey = 4;
	int stackPtr_avl = 0;
	
	/* Application Code */
	/* Create Tree */
	root = TreeGen(myHeap, fixedStack, stackPtr_avl, 9);
	//printf("Tree Generated\n");
	printf("root = %d\n", root);
	
	printTree(myHeap, root);
	printf("key = %d, height = %d, leftPtr = %d, rightPtr = %d\n", myHeap[root+DATA_OFFSET], myHeap[root+HEIGHT_OFFSET], myHeap[root+LEFT_OFFSET], myHeap[root+RIGHT_OFFSET]);
/*	int shift;
	shift = 4;
	printf("key = %d, height = %d, leftPtr = %d, rightPtr = %d\n", myHeap[root+DATA_OFFSET+shift], myHeap[root+HEIGHT_OFFSET+shift], myHeap[root+LEFT_OFFSET+shift], myHeap[root+RIGHT_OFFSET+shift]);	
	shift = 8;
	printf("key = %d, height = %d, leftPtr = %d, rightPtr = %d\n", myHeap[root+DATA_OFFSET+shift], myHeap[root+HEIGHT_OFFSET+shift], myHeap[root+LEFT_OFFSET+shift], myHeap[root+RIGHT_OFFSET+shift]);		
	shift = 12;
	printf("key = %d, height = %d, leftPtr = %d, rightPtr = %d\n", myHeap[root+DATA_OFFSET+shift], myHeap[root+HEIGHT_OFFSET+shift], myHeap[root+LEFT_OFFSET+shift], myHeap[root+RIGHT_OFFSET+shift]);	
*/

	/* Update node */
	
	root = UpdateNode(myHeap, fixedStack, stackPtr_avl, root, 7,10);
	printTree(myHeap, root);
	//	printf("key = %d, height = %d, leftPtr = %d, rightPtr = %d\n", myHeap[root+DATA_OFFSET], myHeap[root+HEIGHT_OFFSET], myHeap[root+LEFT_OFFSET], myHeap[root+RIGHT_OFFSET]);

	//	int shift;
	//shift = 4;
	//printf("key = %d, height = %d, leftPtr = %d, rightPtr = %d\n", myHeap[root+DATA_OFFSET+shift], myHeap[root+HEIGHT_OFFSET+shift], myHeap[root+LEFT_OFFSET+shift], myHeap[root+RIGHT_OFFSET+shift]);	
	//shift = 8+4;
	//printf("key = %d, height = %d, leftPtr = %d, rightPtr = %d\n", myHeap[root+DATA_OFFSET+shift], myHeap[root+HEIGHT_OFFSET+shift], myHeap[root+LEFT_OFFSET+shift], myHeap[root+RIGHT_OFFSET+shift]);		
	
	/* Delete Tree */
	root = DeleteTree(myHeap, fixedStack, stackPtr_avl, root);
	printTree(myHeap,root);
	
	return 0;	
}

