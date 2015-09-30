/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* XILINX CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
//#include "xlistip.h"

#define MMU_BASE 0x43C00000 // change this to your SysAlloc base address
#define MMU_TOKEN 0
#define MMU_STATUS 4
#define MMU_RESULT 8
#define MMU_CMD 12
#define MMU_FREE 16

void print(char *str);

int hw_malloc(int address);
void hw_mfree(int address);
void init_ddr(void);


typedef volatile int data_t;
typedef volatile int ptr_t;
typedef volatile unsigned next_t;

#define LIST_CORE_BASE 0x43C20000

// struct info
struct node_t{
	data_t data;
	next_t next;
};

int main()
{
    init_platform();
    init_ddr();
    print("Hello World\n\r");

    putnum(Xil_In32(0x10000004));
    print("\n\r");

    //int addrArray[10] = {0x10000004, 0x10000014, 0x10000024, 0x10000034, 0x10000044, 0x10000054, 0x10000064, 0x10000074, 0x10000084, 0x10000094};

    Xil_Out32(LIST_CORE_BASE,1);
    int i,j;
    j=0;
    for(i = 0; i < 16000; i++){
    	j ++;
    }

    int returned_value;
    returned_value = Xil_In32(LIST_CORE_BASE + 0x10);
    putnum(returned_value);

    print("Checking results \n");
    int read_data;
    unsigned read_offset;
    int addr_to_check;
    addr_to_check = returned_value;
    for(i = 0;i < 10;i++){
    	putnum(i);
    	print(" ");
    	read_offset = (unsigned)Xil_In32(addr_to_check);
    	read_data = Xil_In32(addr_to_check+4);
    	putnum(addr_to_check);
    	print("offset ");
    	putnum(read_offset);
    	print(", data ");
    	putnum(read_data);
    	print("\n");
    	addr_to_check = addr_to_check + (unsigned)read_offset;
    }

    cleanup_platform();
    return 0;
}

int hw_malloc(int size){
	int token,status,result;
    token = 0;
    status = 0;

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