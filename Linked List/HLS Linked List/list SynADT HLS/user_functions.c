#include "top.h"

/* Creating a list */
ptr_t ListGen(data_t *Master2Mem, data_t *Master2SysAlloc, int SizeOfList, ptr_t hdPtr){
	int i;
	for(i = 0; i < SizeOfList; i++){
		hdPtr = Add2List(Master2Mem, Master2SysAlloc, i, hdPtr);
	}
	return hdPtr+1;
}

/* Performance Measurements */
/* Part 1 */
ptr_t PM_RandListGen(data_t *Master2Mem, data_t *Master2SysAlloc, int log2SizeOfList, ptr_t hdPtr){
	int i;
	int iter = 1 << log2SizeOfList;
	for(i = 0; i < iter; i++){
		hdPtr = Add2List(Master2Mem, Master2SysAlloc, RandGen(log2SizeOfList), hdPtr);
	}
	return hdPtr +1;
}
/* Part 2 */
ptr_t PM_ReverseList(data_t *Master2Mem,ptr_t hdPtr){
	hdPtr = ReverseList(Master2Mem, hdPtr);
	return hdPtr +1;
}
/* Part 3 */
ptr_t PM_DeleteList(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdPtr){
	hdPtr = DeleteList(Master2Mem, Master2SysAlloc, hdPtr);
	return hdPtr +1;
}

