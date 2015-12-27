#include <stdio.h>
#include <stdlib.h>

/* SysAlloc */
#define MMU_BASE 0x43C00000 // change this to your SysAlloc base address
#define MMU_TOKEN 0
#define MMU_STATUS 4
#define MMU_RESULT 8
#define MMU_CMD 12
#define MMU_FREE 16
#define MMU_INIT_RANGE 24
#define MMU_INIT 28

int hw_malloc(int size);
void hw_mfree(int address);
void SysAlloc_init(void);
