#include "SynADT.h"

struct stack_t avlStack(int *myStack, int hdPtr_avl, volatile int command, ptr_t pointer, int volatile operation){
	struct stack_t output;
	int rightMask = 0x80000000;
	volatile int nodeData;
	volatile int stackOut;
	volatile int temp = 0;
	volatile int temp_r = 100;
	volatile int command_in = command;
	if(command == WRITE_STACK){
		if(operation == GOING_LEFT){
			nodeData = pointer;
		}else if(operation == GOING_RIGHT){
			nodeData = pointer | rightMask;
		}
		//hdPtr_avl++;
		temp = hdPtr_avl + 1;
		if(temp != 0){
			//hdPtr_avl = temp;
			myStack[temp] = nodeData;
			output.hdPtr_avl = temp;
		}
	}else{
		//read 
		stackOut = myStack[hdPtr_avl];
		//hdPtr_avl --;
		temp_r = hdPtr_avl - 1;
		if((stackOut & 0x80000000) == GOING_LEFT){
			output.operation = GOING_LEFT;
		}else{
			output.operation = GOING_RIGHT;
		}
		output.pointer = (stackOut & 0x1FFFFFFF);
		//delete
		if(temp_r != 100){
			output.hdPtr_avl = temp_r;
		}
	}
	return output;	
}

volatile struct stack_t AVL_STACK_WRITE(int *myStack, int hdStack, ptr_t pointer, int operation){
	struct stack_t output;
	int nodeData;
	if(operation == GOING_LEFT){
		nodeData = pointer;
	}else if(operation == GOING_RIGHT){
		nodeData = pointer | 0x80000000;
	}
	myStack[hdStack + 1] = nodeData;
	output.hdPtr_avl = hdStack + 1;
	return output;
}
volatile struct stack_t AVL_STACK_READ(int *myStack, int hdStack){
	struct stack_t output;
	int stackOut = myStack[hdStack];
	if((stackOut & 0x80000000) == 0){
		output.operation = GOING_LEFT;
	}else{
		output.operation = GOING_RIGHT;
	}
	output.pointer = (stackOut & 0x1FFFFFFF);
	output.hdPtr_avl = hdStack - 1;
	return output;

}
