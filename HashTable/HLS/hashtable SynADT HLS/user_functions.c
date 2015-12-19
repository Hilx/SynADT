#include "top.h"

ptr_t PM_1_INSERTION(data_t *Master2Mem, data_t *Master2SysAlloc, int log2size){
	// create table
	ptr_t hdTable = CreateHashTable(Master2Mem, Master2SysAlloc, HASH_TABLE_SIZE);	
	// insert keys
	if(TestCase == USE_SEQUENTIAL_KEYS){
		int m = log2size - 1;
		int iter = 1 << m;
		int i;
		for(i = 0; i < iter; i++){
			int key = 16*(i+1);
			InsertNodeHashTable(Master2Mem, Master2SysAlloc, hdTable, key);
		}
	}else{
		int log2_RNG_num_range = log2size - 1; // generate half the range from RNG
		int IterationRange = 1 << log2_RNG_num_range;
		int i;
		for(i = 0; i < IterationRange; i++){
			int key = RandGen(log2_RNG_num_range);
			InsertNodeHashTable(Master2Mem, Master2SysAlloc, hdTable, key);
		}
	}
	return hdTable+1;
}

ptr_t PM_2_CHECK_INSERTION(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdTable, int log2size){
	if(TestCase == USE_SEQUENTIAL_KEYS){
		int m = log2size - 1;
		int iter = 1 << m;
		int i;
		for(i = 0; i < iter; i++){
			int key = RandGen(log2size+2);
			CheckInsert_HashTable(Master2Mem, Master2SysAlloc, hdTable, key);
		}
	}else{
		int log2_RNG_num_range = log2size; // generate FULL range from RNG
		int IterationRange = 1 << (log2_RNG_num_range - 1); // only get half of them
		int i;
		for(i = 0; i < IterationRange; i++){
			int key = RandGen(log2_RNG_num_range);
			CheckInsert_HashTable(Master2Mem, Master2SysAlloc, hdTable, key);
		}
	}	
	return hdTable+1;
}

ptr_t PM_3_UPDATE(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdTable, int log2size){
	if(TestCase == USE_SEQUENTIAL_KEYS){
		int m = log2size - 1;
		int iter = 1 << m;
		int i;
		for(i = 0; i < iter; i++){
			int oldKey = RandGen(log2size+2);
			int newKey = RandGen(log2size+3);
			UpdateNode_HashTable(Master2Mem, Master2SysAlloc, hdTable, oldKey, newKey);
		}
	}else{
		int IterationRange = 1 << (log2size - 1);
		int i;
		for(i = 0; i < IterationRange; i++){
			int oldKey = RandGen(log2size - 1);
			int newKey = RandGen(log2size);
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
