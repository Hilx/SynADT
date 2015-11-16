#include <stdio.h>
#include <stdlib.h>

typedef int ptr_t;

/* Heap */
#define HEAP_SIZE 1000 
#define NULL_PTR 1000

/* SysAlloc */
int myHeapIndex;
int SysMalloc(int size);
void SysFree(int addr);

/* Vectors */
#define VECTOR_VOLUMN 0
#define VECTOR_SIZE 1
#define VECTOR_CONTENT 2
#define FIT_OK 0
#define FIT_NO 1

/* appplication  */
ptr_t VectorInit(int *myHeap, ptr_t hdVector, int size);


/* vector */
int VectorCheckFit(ptr_t hdVector, int maxVolumn, int currentVecSize);
ptr_t VectorCreate(int *myHeap);
ptr_t VectorRealloc(int *myHeap, int maxVolumn, int currentVecSize);
ptr_t VectorPushBack(int *myHeap, ptr_t hdVector, int data);
void VectorPopBack(int *myHeap, ptr_t hdVector);
ptr_t VectorInsert(int *myHeap, ptr_t hdVector, int keyData, int newData);
void VectorDelete(int *myHeap, ptr_t hdVector, int keyData);
ptr_t VectorDestroy(int *myHeap, ptr_t hdVector);

/* memory access functions */
int vector_read_volumn(int *myHeap, ptr_t hdVector);
int vector_read_size(int *myHeap, ptr_t hdVector);
int vector_read_data(int *myHeap, ptr_t hdVector, int index);

void vector_write_volumn(int *myHeap, ptr_t hdVector, int volumn);
void vector_write_size(int *myHeap, ptr_t hdVector, int index);
void vector_write_data(int *myHeap, ptr_t hdVector, int index, int data);

/* software only */
void PrintVec(int *myHeap, ptr_t hdVector);