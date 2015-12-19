#include "top.h"

int CHashTable(data_t *SlaveAXI, data_t *Master2Mem, data_t *Master2SysAlloc, data_t in_size, data_t in_case){

	#pragma HLS INTERFACE s_axilite port=in_size bundle=SlavePort	
	#pragma HLS INTERFACE s_axilite port=in_case bundle=SlavePort
	#pragma HLS INTERFACE s_axilite port=SlaveAXI bundle=SlavePort
	#pragma HLS INTERFACE s_axilite port=return   bundle=SlavePort
	#pragma HLS INTERFACE m_axi depth=1 port=Master2Mem offset=off
	#pragma HLS INTERFACE m_axi depth=1 port=Master2SysAlloc offset=off

	/* -- Control -- */ 
	TestCase = in_case;
	int log2size = in_size;
	
	/* -- init -- */
	table_init(log2size);
	
	ptr_t hdTable = NULL_PTR;
	ptr_t hdTable_old;
	
	Master2SysAlloc[(COUNTER_BASE)/4 + 1] = C_START;
	hdTable_old = hdTable;
	hdTable = PM_1_INSERTION(Master2Mem, Master2SysAlloc, log2size);
	if(hdTable != hdTable_old){
		Master2SysAlloc[(COUNTER_BASE)/4 + 1] = C_STOP;
		hdTable--;
	}
	
	Master2SysAlloc[(COUNTER_BASE)/4 + 2] = C_START;
	hdTable_old = hdTable;
	hdTable = PM_2_CHECK_INSERTION(Master2Mem, Master2SysAlloc, hdTable, log2size);
	if(hdTable != hdTable_old){
		Master2SysAlloc[(COUNTER_BASE)/4 + 2] = C_STOP;
		hdTable--;
	}
	
	Master2SysAlloc[(COUNTER_BASE)/4 + 3] = C_START;
	hdTable_old = hdTable;
	hdTable = PM_3_UPDATE(Master2Mem, Master2SysAlloc, hdTable, log2size);
	if(hdTable != hdTable_old){
		Master2SysAlloc[(COUNTER_BASE)/4 + 3] = C_STOP;
		hdTable--;
	}	
	
	Master2SysAlloc[(COUNTER_BASE)/4 + 4] = C_START;
	hdTable_old = hdTable;
	hdTable = PM_4_DELETION(Master2Mem, Master2SysAlloc, hdTable);
	if(hdTable != hdTable_old){
		Master2SysAlloc[(COUNTER_BASE)/4 + 4] = C_STOP;
		hdTable--;
	}
	
	return hdTable;
}
