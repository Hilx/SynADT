#ifndef PROJECNAME_HEADER_
#define PROJECNAME_HEADER_
#include "SynADT_malloc_wrapper.h"

typedef int ptr_t;  //pointer

#define NULL_PTR 0

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
