#ifndef PROJECNAME_HEADER_
#define PROJECNAME_HEADER_

typedef volatile int data_t; // data
typedef int ptr_t;  //pointer
typedef unsigned next_t; //offset
typedef int entry_index_t;

#define NULL_PTR 0
#define HASH_TABLE_SIZE 10
#define SEARCH_NOT_FOUND 0xFFFFFFFF

/* ----------------------- SysAlloc ------------------- */
#define MMU_BASE 0x43C00000
#define MMU_TOKEN 0
#define MMU_STATUS 4
#define MMU_RESULT 8
#define MMU_CMD 12
#define MMU_FREE 16

volatile int SysMalloc(int size, data_t *Master2SysAlloc);
volatile void SysFree(int addr, data_t *Master2SysAlloc);

/* ----------------------- List ----------------------- */
#define LIST_DATA_OFFSET 1
#define LIST_NEXT_OFFSET 0

struct list_node_t{
	data_t data;
	next_t next;
};

/* Base Functions */
/* Memory Node Access - Write Node */
void list_node_write_next(data_t *Master2Mem, int nodePtr, unsigned offset);
void list_node_write_data(data_t *Master2Mem, int nodePtr, data_t data);
void list_node_write(data_t *Master2Mem, int nodePtr, struct list_node_t currentNode);
/* Memory Node Access - Read Node */
next_t list_node_read_next(data_t *Master2Mem, int nodePtr);
data_t list_node_read_data(data_t *Master2Mem, int nodePtr);
struct list_node_t list_node_read(data_t *Master2Mem, int nodePtr);
/* Low-level list manipulation functions */
void list_node_set_next(data_t *Master2Mem, int currentNodePtr, int nextNodePtr);
ptr_t list_node_alloc_new(data_t *Master2Mem, data_t *Master2SysAlloc, data_t data, ptr_t nextNodePtr);
ptr_t list_node_get_next_pointer(data_t *Master2Mem, ptr_t currentNodePtr);
void list_node_delete(data_t *Master2SysAlloc, ptr_t nodePtr);

/* Application Level functions */
ptr_t Add2List(data_t *Master2Mem, data_t *Master2SysAlloc, data_t data, ptr_t hdPtr);
ptr_t DeleteListNode_HT(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdPtr, int key);
ptr_t SearchInList(data_t *Master2Mem, ptr_t hdPtr, int key);

/* ----------------------- HashTable -----------------------*/
ptr_t CreateHashTable(data_t *Master2Mem, data_t *Master2SysAlloc, int size);
entry_index_t HashingAlgorithm(int key);
void InsertNodeHashTable(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdTable, int key);
void DeleteNodeHashTable(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdTable, int key);
ptr_t SearchHashTable(data_t *Master2Mem, ptr_t hdTable, int key);
ptr_t ReadHashTable(data_t *Master2Mem, ptr_t hdTable, ptr_t entryIndex);
void WriteHashTable(data_t *Master2Mem, ptr_t hdTable, ptr_t entryIndex, ptr_t listPtr);
#endif
