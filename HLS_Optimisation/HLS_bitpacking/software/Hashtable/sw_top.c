#include "sw_top.h"

int main(){

	TestCase = USE_RANDOME_KEYS;
	
	int log2_size = 8;
	
	table_init(log2_size);
	
	int *hdTable = PM_1_INSERTION(log2_size);
	PrintHashTable(hdTable);
	
	PM_2_CHECK_INSERTION(hdTable, log2_size);
	PrintHashTable(hdTable);
	
	PM_3_UPDATE(hdTable, log2_size);
	PrintHashTable(hdTable);
	PM_4_DELETION(hdTable);
	
	return 0;
}
