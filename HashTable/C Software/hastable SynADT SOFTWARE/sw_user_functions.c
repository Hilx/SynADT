/* For testing purposes */
#include "sw_top.h"

/* Performance Measurements */
int *PM_1_INSERTION(int log2_tree_size){
	// Create Table
	int *hdTable = CreateHashTable(HASH_TABLE_SIZE);
	// Fill Keys In
	if(TestCase == USE_SEQUENTIAL_KEYS){
		int m = log2_tree_size - 1;
		int iter = 1 << m;
		int i;
		for(i = 0; i < iter; i++){
			int key = 16*(i+1);
			InsertNodeHashTable(hdTable, key);
		}
	}else{
		int log2_RNG_num_range = log2_tree_size - 1; // generate half the range from RNG
		int IterationRange = 1 << log2_RNG_num_range;
		int i;
		for(i = 0; i < IterationRange; i++){
			int key = RandGen(log2_RNG_num_range);
			InsertNodeHashTable(hdTable, key);
		}

	}
	return hdTable;
}

void PM_2_CHECK_INSERTION(int *hdTable, int log2_tree_size){
	if(TestCase == USE_SEQUENTIAL_KEYS){
		int m = log2_tree_size - 1;
		int iter = 1 << m;
		int i;
		for(i = 0; i < iter; i++){
			int key = RandGen(log2_tree_size+2);
			Check_thenInsert(hdTable,key);
		}
	}else{
		int log2_RNG_num_range = log2_tree_size; // generate FULL range from RNG
		int IterationRange = 1 << (log2_RNG_num_range - 1); // only get half of them
		int i;
		for(i = 0; i < IterationRange; i++){
			int key = RandGen(log2_RNG_num_range);
			Check_thenInsert(hdTable,key);
		}
	}	
}

void PM_3_UPDATE(int *hdTable, int log2_tree_size){
	if(TestCase == USE_SEQUENTIAL_KEYS){
		int m = log2_tree_size - 1;
		int iter = 1 << m;
		int i;
		for(i = 0; i < iter; i++){
			int oldKey = RandGen(log2_tree_size+2);
			int newKey = RandGen(log2_tree_size+3);
			UpdateNode(hdTable, oldKey, newKey);
		}
	}else{
		int IterationRange = 1 << (log2_tree_size - 1);
		int i;
		for(i = 0; i < IterationRange; i++){
			int oldKey = RandGen(log2_tree_size - 1);
			int newKey = RandGen(log2_tree_size);
			UpdateNode(hdTable, oldKey, newKey);
		}
	}
}

void PM_4_DELETION(int *hdTable){
	DeleteHashTable(HASH_TABLE_SIZE, hdTable);
}

void table_init(int LOG2_TOTAL){
	if(LOG2_TOTAL > LF_THRESHOLD){
		HASH_TABLE_SIZE = 1 << (LOG2_TOTAL - LOG2_CHAIN);		
	}else{
		HASH_TABLE_SIZE = 10;
	}
}
