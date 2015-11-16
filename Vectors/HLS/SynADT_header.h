#ifndef PROJECNAME_HEADER_
#define PROJECNAME_HEADER_

typedef volatile int data_t; // data
typedef int ptr_t;  //pointer

#define NULL_PTR 0
/* ----------------------- SysAlloc ------------------- */
#define MMU_BASE 0x43C00000
#define MMU_TOKEN 0
#define MMU_STATUS 4
#define MMU_RESULT 8
#define MMU_CMD 12
#define MMU_FREE 16

volatile int SysMalloc(int size, data_t *Master2SysAlloc);
volatile void SysFree(int addr, data_t *Master2SysAlloc);

/* -------------- Vectors ---------------- */
#define VECTOR_VOLUMN 0
#define VECTOR_SIZE 1
#define VECTOR_CONTENT 2
#define FIT_OK 0
#define FIT_NO 1

ptr_t VectorInit(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdVector, int size);
int VectorCheckFit(data_t *Master2Mem, ptr_t hdVector, int maxVolumn, int currentVecSize);
ptr_t VectorCreate(data_t *Master2Mem, data_t *Master2SysAlloc);
ptr_t VectorRealloc(data_t *Master2Mem, data_t *Master2SysAlloc, int maxVolumn, int currentVecSize);
ptr_t VectorPushBack(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdVector, int data);
void VectorPopBack(data_t *Master2Mem, ptr_t hdVector);
ptr_t VectorInsert(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdVector, int keyData, int newData);
void VectorDelete(data_t *Master2Mem, ptr_t hdVector, int keyData);
ptr_t VectorDestroy(data_t *Master2SysAlloc, ptr_t hdVector);

/* memory access functions */
int vector_read_volumn(data_t *Master2Mem, ptr_t hdVector);
int vector_read_size(data_t *Master2Mem, ptr_t hdVector);
int vector_read_data(data_t *Master2Mem, ptr_t hdVector, int index);

void vector_write_volumn(data_t *Master2Mem, ptr_t hdVector, int volumn);
void vector_write_size(data_t *Master2Mem, ptr_t hdVector, int index);
void vector_write_data(data_t *Master2Mem, ptr_t hdVector, int index, int data);
#endif
