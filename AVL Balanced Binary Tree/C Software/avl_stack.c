#include "main.h"

struct stack_t avlStack(int *myStack, int hdPtr_avl, int command, ptr_t pointer, int operation){
	struct stack_t output;
	int rightMask = 0x80000000;
	int nodeData;
	int stackOut;
	if(command == WRITE_STACK){
		//printf("Writing stack [ %d ]\n", pointer);
		if(operation == GOING_LEFT){
			nodeData = pointer;
		}else if(operation == GOING_RIGHT){
			nodeData = pointer | rightMask;
		}

	//output.hdPtr = Add2List(nodeData, hdPtr);	
	hdPtr_avl++;
	myStack[hdPtr_avl] = nodeData;
	output.hdPtr_avl = hdPtr_avl;
		
	}else if(command == READ_STACK){
		//read 
		//stackOut = hdPtr[0];
		stackOut = myStack[hdPtr_avl];
		hdPtr_avl--;
		if((stackOut & 0x80000000) == GOING_LEFT){
			output.operation = GOING_LEFT;
		}else{
			output.operation = GOING_RIGHT;
		}
		output.pointer = (stackOut & 0x7FFFFFFF);
		//printf("Reading stack [ %d ]\n", output.pointer);
		//delete
		//output.hdPtr = DeleteNode(hdPtr);
		output.hdPtr_avl = hdPtr_avl;
	}
	
	return output;	
}