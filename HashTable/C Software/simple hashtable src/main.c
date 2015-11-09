#include "main.h"

int main(){
	int *myHeap = (int*)malloc(sizeof(int) * HEAP_SIZE);
	ptr_t hdTable;
	
	hdTable = CreateHashTable(myHeap, HASH_TABLE_SIZE);
	printf("created\n");
	InsertNodeHashTable(myHeap, hdTable, 3);
	InsertNodeHashTable(myHeap, hdTable, 13);
	//DeleteNodeHashTable(myHeap, hdTable, 13);
	
	ptr_t resultPtr = SearchHashTable(myHeap, hdTable,13);
	printf("search found node with value %d\n", list_node_read_data(myHeap, resultPtr));

	return 0;
}
