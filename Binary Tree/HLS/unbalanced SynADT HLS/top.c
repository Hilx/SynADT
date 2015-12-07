#include "top.h"

int CTreeUBnew(data_t *SlaveAXI, data_t *Master2Mem, data_t *Master2SysAlloc, data_t input_r, data_t func_sel){

	#pragma HLS INTERFACE s_axilite port=func_sel bundle=SlavePort
	#pragma HLS INTERFACE s_axilite port=input_r bundle=SlavePort
	#pragma HLS INTERFACE s_axilite port=SlaveAXI bundle=SlavePort
	#pragma HLS INTERFACE s_axilite port=return   bundle=SlavePort
	#pragma HLS INTERFACE m_axi depth=1 port=Master2Mem offset=off
	#pragma HLS INTERFACE m_axi depth=1 port=Master2SysAlloc offset=off

	int log2_tree_size = input_r;

	ptr_t root = NULL_PTR;
	ptr_t stackPtr = NULL_PTR;

	ptr_t root_old;

	Master2SysAlloc[(COUNTER_BASE)/4 + 1] = C_START;
	root_old = root;
	root =  PM_1_INSERTION(Master2Mem, Master2SysAlloc, stackPtr, root, log2_tree_size);
	if(root != root_old){
		Master2SysAlloc[(COUNTER_BASE)/4 + 1] = C_STOP;
		root--;
	}

	Master2SysAlloc[(COUNTER_BASE)/4 + 2] = C_START;
	root_old = root;
	root =  PM_2_CHECK_INSERTION(Master2Mem, Master2SysAlloc, stackPtr, root, log2_tree_size);
	if(root != root_old){
		Master2SysAlloc[(COUNTER_BASE)/4 + 2] = C_STOP;
		root--;
	}

	//if(func_sel == 1){
		Master2SysAlloc[(COUNTER_BASE)/4 + 3] = C_START;
		root_old = root;

		root =  PM_3_UPDATE(Master2Mem, Master2SysAlloc, stackPtr, root, log2_tree_size);
		if(root != root_old){
			Master2SysAlloc[(COUNTER_BASE)/4 + 3] = C_STOP;
			root--;
		}
	//}

	Master2SysAlloc[(COUNTER_BASE)/4 + 4] = C_START;
	root_old = root;
	root =  PM_4_DELETION(Master2Mem, Master2SysAlloc, stackPtr, root);
	if(root != root_old){
		Master2SysAlloc[(COUNTER_BASE)/4 + 4] = C_STOP;
		root--;
	}
	
	return root;
}
