#include "SynADT_header.h"

struct stack_t staticStack(int *myStack, int hdPtr_avl, int command, ptr_t pointer, int operation){
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
	hdPtr_avl++;
	myStack[hdPtr_avl] = nodeData;
	output.hdPtr_avl = hdPtr_avl;		
	}else if(command == READ_STACK){
		//read 
		stackOut = myStack[hdPtr_avl];
		hdPtr_avl--;
		if((stackOut & 0x80000000) == GOING_LEFT){
			output.operation = GOING_LEFT;
		}else{
			output.operation = GOING_RIGHT;
		}
		output.pointer = (stackOut & 0x7FFFFFFF);
		//delete
		output.hdPtr_avl = hdPtr_avl;
	}
	return output;	
}