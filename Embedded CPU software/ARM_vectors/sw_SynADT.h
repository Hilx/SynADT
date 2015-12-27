#include "sw_SynADT_malloc_wrapper.h"

/* Vectors */
#define VECTOR_VOLUMN 0
#define VECTOR_SIZE 1
#define VECTOR_CONTENT 2
#define FIT_OK 0
#define FIT_NO 1

/* appplication  */
int *VectorInit(int *hdVector, int size);

/* vector */
int VectorCheckFit(int *hdVector, int maxVolumn, int currentVecSize);
int *VectorCreate(void);
int *VectorRealloc(int maxVolumn, int currentVecSize);
int *VectorPushBack(int *hdVector, int data);
void VectorPopBack(int *hdVector);
int *VectorInsert(int *hdVector, int keyData, int newData);
void VectorDelete(int *hdVector, int keyData);
int *VectorDestroy(int *hdVector);

/* memory access functions */
int vector_read_volumn(int *hdVector);
int vector_read_size(int *hdVector);
int vector_read_data(int *hdVector, int index);

void vector_write_volumn(int *hdVector, int volumn);
void vector_write_size(int *hdVector, int index);
void vector_write_data(int *hdVector, int index, int data);

/* software only */
void PrintVec(int *hdVector);