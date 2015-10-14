#include "top.h"

int CTreeUB(data_t *SlaveAXI, data_t *Master2Mem, data_t *Master2SysAlloc){

	#pragma HLS INTERFACE s_axilite port=SlaveAXI bundle=SlavePort
	#pragma HLS INTERFACE s_axilite port=return   bundle=SlavePort
	#pragma HLS INTERFACE m_axi depth=1 port=Master2Mem offset=off
	#pragma HLS INTERFACE m_axi depth=1 port=Master2SysAlloc offset=off

	ptr_t root;
	ptr_t stackPtr = NULL_PTR;
	int NumberOfNodes = 7;
	int searchKey = 7;
	ptr_t searchResultPtr;

	root = NULL_PTR;
	/* Create */
	root = TreeGen(Master2Mem, Master2SysAlloc, stackPtr, NumberOfNodes);
	/* Update */
	root = UpdateNode(Master2Mem, Master2SysAlloc, stackPtr, root, 3, 10);
	/* Delete */
	root = DeleteTree(Master2Mem, Master2SysAlloc, stackPtr, root);



	return root;
}
