#include "top.h"

ptr_t PM_1_INSERTION(data_t *Master2Mem, data_t *Master2SysAlloc, int log2size){
	// create table
	ptr_t hdTable = CreateHashTable(Master2Mem, Master2SysAlloc, HASH_TABLE_SIZE);	
	// insert keys
	int i;
	int mask_range = log2size - 1;
	int iteration_range = 1 << mask_range;
	int key;
	if(TestCase == USE_SEQUENTIAL_KEYS){
SEQ: for(i = 0; i < iteration_range; i++){
			key = 16*(i+1);
			InsertNodeHashTable(Master2Mem, Master2SysAlloc, hdTable, key);
		}
	}else{
RAN: for(i = 0; i < iteration_range; i++){
			key = RandGen(mask_range);
			InsertNodeHashTable(Master2Mem, Master2SysAlloc, hdTable, key);
		}
	}
	return hdTable+1;
}

ptr_t PM_2_CHECK_INSERTION(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdTable, int log2size){
	int i;
	int mask_range;
	int iteration_range = 1 << (log2size - 1);
	int key;
	if(TestCase == USE_SEQUENTIAL_KEYS){
		mask_range = log2size + 2;
SEQ: for(i = 0; i < iteration_range; i++){
			key = RandGen(mask_range);
			CheckInsert_HashTable(Master2Mem, Master2SysAlloc, hdTable, key);
		}
	}else{
		mask_range = log2size;
RAN: for(i = 0; i < iteration_range; i++){
			key = RandGen(mask_range);
			CheckInsert_HashTable(Master2Mem, Master2SysAlloc, hdTable, key);
		}
	}	
	return hdTable+1;
}

ptr_t PM_3_UPDATE(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdTable, int log2size){
	int i, oldKey, newKey;
	int iteration_range = 1 << (log2size - 1);
	if(TestCase == USE_SEQUENTIAL_KEYS){
		for(i = 0; i < iteration_range; i++){
			oldKey = RandGen(log2size+2);
			newKey = RandGen(log2size+3);
			UpdateNode_HashTable(Master2Mem, Master2SysAlloc, hdTable, oldKey, newKey);
		}
	}else{
		for(i = 0; i < iteration_range; i++){
			oldKey = RandGen(log2size - 1);
			newKey = RandGen(log2size);
			UpdateNode_HashTable(Master2Mem, Master2SysAlloc, hdTable, oldKey, newKey);
		}
	}	
	return hdTable+1;
}

ptr_t PM_4_DELETION(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdTable){	
	DeleteHashTable(Master2Mem, Master2SysAlloc, HASH_TABLE_SIZE, hdTable);
	return hdTable+1;
}

void table_init(int LOG2_TOTAL){
	if(LOG2_TOTAL > LF_THRESHOLD){
		HASH_TABLE_SIZE = 1 << (LOG2_TOTAL - LOG2_CHAIN);		
	}else{
		HASH_TABLE_SIZE = 10;
	}
}
