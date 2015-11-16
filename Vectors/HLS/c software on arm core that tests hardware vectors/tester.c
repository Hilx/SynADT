#include <stdio.h>
#include "platform.h"

#define MMU_BASE 0x43C00000 // change this to your SysAlloc base address
#define MMU_TOKEN 0
#define MMU_STATUS 4
#define MMU_RESULT 8
#define MMU_CMD 12
#define MMU_FREE 16

void print(char *str);

int hw_malloc(int size);
void hw_mfree(int address);
void init_ddr(void);

typedef volatile int data_t;
typedef volatile int ptr_t;
typedef volatile unsigned next_t;

#define VECTORS_BASE 0x43C10000

int main()
{
    init_platform();
    init_ddr();
    print("Hello World\n\r");

    //Send command to vectors_ip
    Xil_Out32(VECTORS_BASE + 0x20, 7);

    //Start vectors_ip
    Xil_Out32(VECTORS_BASE,1);

    //Check result
    int hdVector;
    hdVector = Xil_In32(VECTORS_BASE + 0x10);

    while(hdVector == 0){
    	hdVector = Xil_In32(VECTORS_BASE + 0x10);
    }

    print("hdVector =");
    putnum(hdVector);
    print("\n");

    int vectorVolumn;
    vectorVolumn = Xil_In32(hdVector);
    print("vectorVolumn ");
    putnum(vectorVolumn);

    int vectorSize;
    vectorSize = Xil_In32(hdVector+4);
    print("\nvectorSize ");
    putnum(vectorSize);

    print("\nVector [");
    int i;
    for(i = 0; i < vectorSize; i++){
    	putnum(Xil_In32(hdVector + 8 + i*4));
    	print(" ");
    }
    print("]\n");

    cleanup_platform();
    return 0;
}

int hw_malloc(int size){
	int token,status,result;
    token = 0;
    status = 0;
    print("MALLOCING \n");
	// acquiring token
	while(token == 0){
		token =  Xil_In32(MMU_BASE + MMU_TOKEN);
	}

	// write request
	Xil_Out32(MMU_BASE + MMU_CMD, size);

	// checking allocation status
	while(status == 0){
    	status =  Xil_In32(MMU_BASE + MMU_STATUS);
	}

	//read result back
    result = Xil_In32(MMU_BASE + MMU_RESULT);

    return result;
}

void hw_mfree(int address){

	int token,status,result;
    token = 0;
    status = 0;

	// acquiring token
	while(token == 0){
		token =  Xil_In32(MMU_BASE + MMU_TOKEN);
	}

	// write request
	Xil_Out32(MMU_BASE + MMU_FREE, address);

	// checking de-allocation status
	while(status == 0){
    	status =  Xil_In32(MMU_BASE + MMU_STATUS);
	}

	// performing a READ to reset token
    result = Xil_In32(MMU_BASE + MMU_RESULT);
}

void init_ddr(void){
	int i;
	for(i = 0; i <2396745; i++)
		Xil_Out32(0x18000000 + 4*i, 0);

}
