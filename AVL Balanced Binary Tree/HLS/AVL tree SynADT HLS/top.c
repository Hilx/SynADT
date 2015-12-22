#include "top.h"

int CTreeAVL(data_t *SlaveAXI, data_t *Master2Mem, data_t *Master2SysAlloc, data_t SIZE_IN, data_t CASE_SEL){
	#pragma HLS INTERFACE s_axilite port=SlaveAXI bundle=SlavePort
	#pragma HLS INTERFACE m_axi depth=1 port=Master2Mem offset=off
	#pragma HLS INTERFACE m_axi depth=1 port=Master2SysAlloc offset=off		
	#pragma HLS INTERFACE s_axilite port=SIZE_IN bundle=SlavePort
	#pragma HLS INTERFACE s_axilite port=CASE_SEL bundle=SlavePort
	#pragma HLS INTERFACE s_axilite port=return   bundle=SlavePort

	data_t fixedStackImpl[33];
	data_t fixedStackImpl_second[33];
	#pragma HLS RESOURCE variable=fixedStackImpl core=RAM_1P
	data_t *fixedStack;	
	fixedStack = fixedStackImpl;	
	data_t *secondStack = fixedStackImpl_second;
	
	ptr_t root, stackPtr_avl, root_old;
	
	TestCase = CASE_SEL;
	structure_size = SIZE_IN;

	root = NULL_PTR;
	stackPtr_avl = NULL_PTR;
	
	xor_rng_init();

	// phase 1
	Master2SysAlloc[(COUNTER_BASE)/4 + 1] = C_START;
	root_old = root;
	root = PM_1_INSERTION(Master2Mem, Master2SysAlloc, fixedStack, stackPtr_avl, structure_size);
	if(root != root_old){
		Master2SysAlloc[(COUNTER_BASE)/4 + 1] = C_STOP;
		root--;
	}
	// phase 2
	Master2SysAlloc[(COUNTER_BASE)/4 + 2] = C_START;
	root_old = root;
	root = PM_2_CHECK_INSERTION(Master2Mem, Master2SysAlloc, fixedStack, stackPtr_avl, root, structure_size);
	if(root != root_old){
		Master2SysAlloc[(COUNTER_BASE)/4 + 2] = C_STOP;
		root--;
	}

	// phase 3
	Master2SysAlloc[(COUNTER_BASE)/4 + 3] = C_START;
	root_old = root;
	root = PM_3_UPDATE(Master2Mem, Master2SysAlloc, fixedStack, secondStack, stackPtr_avl, root, structure_size);
	if(root != root_old){
		Master2SysAlloc[(COUNTER_BASE)/4 + 3] = C_STOP;
		root--;
	}

	// phase4
	Master2SysAlloc[(COUNTER_BASE)/4 + 4] = C_START;
	root_old = root;
	root = PM_4_DELETION(Master2Mem, Master2SysAlloc, fixedStack, stackPtr_avl, root);
	if(root != root_old){
		Master2SysAlloc[(COUNTER_BASE)/4 + 4] = C_STOP;
		root--;
	}

	return root;
}

