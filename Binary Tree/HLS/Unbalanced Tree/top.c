#include "top.h"

int ubtree_ip(data_t *SlaveAXI, data_t *Master2Mem, data_t *Master2SysAlloc){

	#pragma HLS INTERFACE s_axilite port=SlaveAXI bundle=SlavePort
	#pragma HLS INTERFACE s_axilite port=return   bundle=SlavePort
	#pragma HLS INTERFACE m_axi depth=1 port=Master2Mem offset=off
	#pragma HLS INTERFACE m_axi depth=1 port=Master2SysAlloc offset=off

	ptr_t root = NULL_PTR;	
	ptr_t stackPtr = NULL_PTR;
	int NumberOfNodes = 10;
	int searchKey = 7;
	ptr_t searchResultPtr;

	/* Create */
	root = TreeGen(Master2Mem, Master2SysAlloc, NumberOfNodes, stackPtr);
	/* Update */
	//root = UpdateNode(Master2Mem, Master2SysAlloc, stackPtr, root, 3, 10);
	/* Delete */
	//root = DeleteTree(Master2Mem, Master2SysAlloc, stackPtr, root);
		
	return root;
}
