#include "sw_top.h"

int main(void){
	int *hdList = NULL;
	
	hdList = RandListGen(5, hdList);
	PrintList(hdList);

	hdList = ReverseList(hdList);
	PrintList(hdList);
	
	hdList = DeleteList(hdList);
	PrintList(hdList);
}