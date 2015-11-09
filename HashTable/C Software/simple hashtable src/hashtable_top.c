#include "hashtable.h"

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

ptr_t CreateHashTable(int *myHeap, int size){
	ptr_t hdTable = SysMalloc(size);	
	ptr_t hdList = NULL_PTR;
	int i;
	for(i = 0; i < size; i++){
		WriteHashTable(myHeap, hdTable, i, hdList);		
	}	
	return hdTable;
}

entry_index_t HashingAlgorithm(int key){
	return key % HASH_TABLE_SIZE;
}

void InsertNodeHashTable(int *myHeap, ptr_t hdTable, int key){
	entry_index_t entryIndex = HashingAlgorithm(key);
	ptr_t hdList = ReadHashTable(myHeap, hdTable, entryIndex);	
	// read from hash table
	hdList = Add2List(myHeap, key, hdList);
	PrintList(myHeap, hdList);
	// write/update hash table entry pointer value
	WriteHashTable(myHeap, hdTable, entryIndex, hdList);	
}

void DeleteNodeHashTable(int *myHeap, ptr_t hdTable, int key){
	entry_index_t entryIndex = HashingAlgorithm(key);
	ptr_t hdList = ReadHashTable(myHeap, hdTable, entryIndex);	
	// read from hash table
	hdList = DeleteListNode(myHeap, hdList, key);
	PrintList(myHeap, hdList);
	// write/update hash table entry pointer value
	WriteHashTable(myHeap, hdTable, entryIndex, hdList);	
}

ptr_t SearchHashTable(int *myHeap, ptr_t hdTable, int key){
	entry_index_t entryIndex = HashingAlgorithm(key);
	ptr_t hdList = ReadHashTable(myHeap, hdTable, entryIndex);	
	return SearchInList(myHeap, hdList, key);
}

/* basic stuff */
ptr_t ReadHashTable(int *myHeap, ptr_t hdTable, ptr_t entryIndex){
	return myHeap[hdTable + entryIndex];
}
void WriteHashTable(int *myHeap, ptr_t hdTable, ptr_t entryIndex, ptr_t listPtr){
	myHeap[hdTable + entryIndex] = listPtr;
}