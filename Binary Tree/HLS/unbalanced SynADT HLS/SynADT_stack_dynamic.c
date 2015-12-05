#include "SynADT.h"

struct stack_t dynamicStack(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdPtr, int command, ptr_t pointer, int operation){
	struct stack_t output;
	int rightMask = 0x80000000;
	int nodeData;
	int stackOut;
	if(command == WRITE_STACK){
		if(operation == GOING_LEFT){
			nodeData = pointer;
		}else if(operation == GOING_RIGHT){
			nodeData = pointer | rightMask;
		}

	output.hdPtr = Add2List(Master2Mem, Master2SysAlloc, nodeData, hdPtr);		
		
	}else if(command == READ_STACK){
		//read 
		stackOut = list_node_read_data(Master2Mem, hdPtr);
		if((stackOut & 0x80000000) == GOING_LEFT){
			output.operation = GOING_LEFT;
		}else{
			output.operation = GOING_RIGHT;
		}
		output.pointer = (stackOut & 0x7FFFFFFF);
		//delete
		output.hdPtr = DeleteListNode(Master2Mem, Master2SysAlloc, hdPtr);
	}
	return output;
}