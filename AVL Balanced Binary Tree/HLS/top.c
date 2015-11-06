#include "top.h"

int CTreeAVL(data_t *SlaveAXI, data_t *Master2Mem, data_t *Master2SysAlloc){

	//int input;

	#pragma HLS INTERFACE s_axilite port=SlaveAXI bundle=SlavePort
	//#pragma HLS INTERFACE s_axilite port=input bundle=SlavePort
	#pragma HLS INTERFACE s_axilite port=return   bundle=SlavePort
	#pragma HLS INTERFACE m_axi depth=1 port=Master2Mem offset=off
	#pragma HLS INTERFACE m_axi depth=1 port=Master2SysAlloc offset=off

	ptr_t root = NULL_PTR;
	ptr_t stackPtr = NULL_PTR;
	int NumberOfNodes = 10;
	
	data_t fixedStackImpl[33];
	#pragma HLS RESOURCE variable=fixedStackImpl core=RAM_1P
	data_t *fixedStack;
	
	fixedStack = fixedStackImpl;

	/* Create */
	root = TreeGen(Master2Mem, Master2SysAlloc, fixedStack, stackPtr, NumberOfNodes);
	
	/* Update */
	root = UpdateNode(Master2Mem, Master2SysAlloc, fixedStack, stackPtr, root, 3, 15);

	/* Delete */
	root = DeleteTree(Master2Mem, Master2SysAlloc, fixedStack, stackPtr, root);

	return root;
}
