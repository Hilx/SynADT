/* ----------------------- SysAlloc ------------------- */
#define MMU_BASE 0x43C00000
#define MMU_TOKEN 0
#define MMU_STATUS 4
#define MMU_RESULT 8
#define MMU_CMD 12
#define MMU_FREE 16

typedef volatile int data_t;     // data

volatile int SysMalloc(int size, data_t *Master2SysAlloc);
volatile void SysFree(int addr, data_t *Master2SysAlloc);
