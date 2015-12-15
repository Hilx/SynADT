#include "sw_SynADT.h"

/* Software only */
void PrintHashTable(int *hdTable){
	printf("\n---------- table ----------\n");
	if(hdTable != NULL){
		int i;
		int *hdList;
		for(i = 0; i<HASH_TABLE_SIZE; i++){
			printf("(%d) ", i);
			hdList = ReadHashTable(hdTable, i);	
			PrintList(hdList);
		}
	}else{
		printf("There's no table.\n");
	}
	printf("\n---------- xxxxx ----------\n");
}
/* Check Insert */
void Check_thenInsert(int *hdTable, int key){
	int *search_result = SearchHashTable(hdTable, key);
	if(search_result == NULL){
		InsertNodeHashTable(hdTable, key);
	}
}
/* Update */
void UpdateNode(int *hdTable, int oldKey, int newKey){
	DeleteNodeHashTable(hdTable, oldKey);
	InsertNodeHashTable(hdTable, newKey);
}
/* HashTable */
int *CreateHashTable(int NUM_OF_ENTRY){
	int *hdTable = SysMalloc(NUM_OF_ENTRY*sizeof(int));	
	int *hdList = NULL;
	int i;
	for(i = 0; i < NUM_OF_ENTRY; i++){
		WriteHashTable(hdTable, i, hdList);		
	}	
	return hdTable;
}

void DeleteHashTable(int NUM_OF_ENTRY, int *hdTable){
	int i;
	int *hdList;
	for(i = 0; i < NUM_OF_ENTRY; i++){
		hdList = ReadHashTable(hdTable, i);	
		hdList = DeleteList(hdList);
	}		
	SysFree(hdTable);
}

int HashingAlgorithm(int key){
	return key % HASH_TABLE_SIZE;
}

void InsertNodeHashTable(int *hdTable, int key){
	int entryIndex = HashingAlgorithm(key);
	int *hdList = ReadHashTable(hdTable, entryIndex);	
	hdList = Add2SortedList(key, hdList);
	WriteHashTable(hdTable, entryIndex, hdList);	
}

void DeleteNodeHashTable(int *hdTable, int key){
	int entryIndex = HashingAlgorithm(key);
	int *hdList = ReadHashTable(hdTable, entryIndex);	
	hdList = DeleteListNode_HT(hdList, key);
	WriteHashTable(hdTable, entryIndex, hdList);	
}

int *SearchHashTable(int *hdTable, int key){
	int entryIndex = HashingAlgorithm(key);
	int *hdList = ReadHashTable(hdTable, entryIndex);	
	return SearchInList(hdList, key);
}

/* basic stuff */
int *ReadHashTable(int *hdTable, int entryIndex){
	next_t offset = (next_t)hdTable[entryIndex];
	int *returnPtr;
	int pointerValue = (int)hdTable + offset;
	if(pointerValue == NULL_PTR){
		returnPtr = NULL;
	}else{
		returnPtr = hdTable + offset;
	}

	return returnPtr;
}
void WriteHashTable(int *hdTable, int entryIndex, int *listPtr){
	next_t offset;
	if (listPtr == NULL)
	{
		offset = NULL_PTR - (int)hdTable;
	}else{
		offset = listPtr - hdTable;
	}
	hdTable[entryIndex] = offset;
}

