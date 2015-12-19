#include "SynADT.h"

/* --- matching functions --- */
void CheckInsert_HashTable(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdTable, int key){
	ptr_t search_result = SearchHashTable(Master2Mem, hdTable, key);
	if(search_result == SEARCH_NOT_FOUND){
		InsertNodeHashTable(Master2Mem, Master2SysAlloc, hdTable, key);
	}
}

void UpdateNode_HashTable(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdTable, int oldKey, int newKey){
	DeleteNodeHashTable(Master2Mem, Master2SysAlloc, hdTable, oldKey);
	InsertNodeHashTable(Master2Mem, Master2SysAlloc, hdTable, newKey);
}
/* --------------------- */
ptr_t CreateHashTable(data_t *Master2Mem, data_t *Master2SysAlloc, int size){
	data_t *myHeap = Master2Mem;
	ptr_t hdTable = SysMalloc(size*4, Master2SysAlloc);	
	ptr_t hdList = NULL_PTR;
	int i;
	for(i = 0; i < size; i++){
		WriteHashTable(myHeap, hdTable, i, hdList);		
	}	
	return hdTable;
}

void DeleteHashTable(data_t *Master2Mem, data_t *Master2SysAlloc, int size, ptr_t hdTable){
	int i;
	ptr_t hdList;
	for(i = 0; i < size; i++){
		hdList = ReadHashTable(Master2Mem, hdTable, i);
		DeleteList(Master2Mem, Master2SysAlloc, hdList);
	}		
	SysFree(hdTable, Master2SysAlloc);
}


entry_index_t HashingAlgorithm(int key){
	return key % HASH_TABLE_SIZE;
}

void InsertNodeHashTable(data_t *Master2Mem, data_t *Master2SysAlloc,ptr_t hdTable, int key){
	data_t *myHeap = Master2Mem;
	entry_index_t entryIndex = HashingAlgorithm(key);
	ptr_t hdList = ReadHashTable(myHeap, hdTable, entryIndex);	
	// read from hash table
	hdList = Add2SortedList(myHeap, Master2SysAlloc, key, hdList);
	// write/update hash table entry pointer value
	WriteHashTable(myHeap, hdTable, entryIndex, hdList);	
}

void DeleteNodeHashTable(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdTable, int key){
	data_t *myHeap = Master2Mem;
	entry_index_t entryIndex = HashingAlgorithm(key);
	ptr_t hdList = ReadHashTable(myHeap, hdTable, entryIndex);	
	// read from hash table
	hdList = DeleteListNode_HT(myHeap, Master2SysAlloc, hdList, key);
	// write/update hash table entry pointer value
	WriteHashTable(myHeap, hdTable, entryIndex, hdList);	
}

ptr_t SearchHashTable(data_t *Master2Mem, ptr_t hdTable, int key){
	data_t *myHeap = Master2Mem;
	entry_index_t entryIndex = HashingAlgorithm(key);
	ptr_t hdList = ReadHashTable(myHeap, hdTable, entryIndex);	
	return SearchInList(myHeap, hdList, key);
}

/* basic stuff */
ptr_t ReadHashTable(data_t *Master2Mem, ptr_t hdTable, ptr_t entryIndex){
	data_t *myHeap = Master2Mem;
	return myHeap[hdTable/4 + entryIndex];
}
void WriteHashTable(data_t *Master2Mem, ptr_t hdTable, ptr_t entryIndex, ptr_t listPtr){
	data_t *myHeap = Master2Mem;
	myHeap[hdTable/4 + entryIndex] = listPtr;
}
