#include "top.h"
int Cavl(data_t *SlaveAXI, data_t *Master2Mem, data_t *Master2SysAlloc, data_t input_r, data_t SelectTestCase, volatile ptr_t root_in, data_t key_in){

	#pragma HLS INTERFACE s_axilite port=SelectTestCase bundle=SlavePort
	#pragma HLS INTERFACE s_axilite port=input_r bundle=SlavePort
	#pragma HLS INTERFACE s_axilite port=SlaveAXI bundle=SlavePort
	#pragma HLS INTERFACE s_axilite port=return   bundle=SlavePort
	#pragma HLS INTERFACE m_axi depth=1 port=Master2Mem offset=off
	#pragma HLS INTERFACE m_axi depth=1 port=Master2SysAlloc offset=off

	#pragma HLS INTERFACE s_axilite port=root_in bundle=SlavePort
	#pragma HLS INTERFACE s_axilite port=key_in bundle=SlavePort

	int log2_tree_size = input_r;
	TestCase = SelectTestCase;


	ptr_t root = NULL_PTR;
	//ptr_t root = root_in;
	ptr_t root_old;
	//xor_rng_init();

	int myStack1[40], myStack2[40];
	int *stack1 = myStack1;
	int *stack2 = myStack2;
	
	//int key = key_in;

	//root_old = root;
	//root =  add_new(Master2Mem, Master2SysAlloc, stack1, 0, root, key);




	Master2SysAlloc[(COUNTER_BASE)/4 + 1] = C_START;
	root_old = root;
	root =  PM_1_INSERTION(Master2Mem, Master2SysAlloc, stack1, 0, log2_tree_size);
	if(root != root_old){
		Master2SysAlloc[(COUNTER_BASE)/4 + 1] = C_STOP;
		root--;
	}

	Master2SysAlloc[(COUNTER_BASE)/4 + 2] = C_START;
	root_old = root;
	root =  PM_2_CHECK_INSERTION(Master2Mem, Master2SysAlloc, stack1, 0, root, log2_tree_size);
	if(root != root_old){
		Master2SysAlloc[(COUNTER_BASE)/4 + 2] = C_STOP;
		root--;
	}


	Master2SysAlloc[(COUNTER_BASE)/4 + 3] = C_START;
	root_old = root;
	root =  PM_3_UPDATE(Master2Mem, Master2SysAlloc, stack1, stack2, 0, root, log2_tree_size);
	if(root != root_old){
		Master2SysAlloc[(COUNTER_BASE)/4 + 3] = C_STOP;
		root--;
	}


	Master2SysAlloc[(COUNTER_BASE)/4 + 4] = C_START;
	root_old = root;
	root =  PM_4_DELETION(Master2Mem, Master2SysAlloc, stack1, 0, root);
	if(root != root_old){
		Master2SysAlloc[(COUNTER_BASE)/4 + 4] = C_STOP;
		root--;
	}		


	return root;
}
