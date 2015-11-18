#ifndef PROJECNAME_HEADER_
#define PROJECNAME_HEADER_

typedef volatile int data_t; // data
typedef volatile int ptr_t;  //pointer
typedef volatile signed next_t; //offset

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
/* Creating a list */
ptr_t ListGen(data_t *Master2Mem, data_t *Master2SysAlloc, int SizeOfList, ptr_t hdPtr);
ptr_t Add2List(data_t *Master2Mem, data_t *Master2SysAlloc, data_t data, ptr_t hdPtr);
/* Reverse the list */
ptr_t ReverseList(data_t *Master2Mem, ptr_t hdPtr);
/* Delete the list */
ptr_t DeleteList(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdPtr);
ptr_t DeleteListNode(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdPtr);

/* ----------------------- Tree ----------------------- */
#define TREE_DATA_OFFSET 3
#define TREE_LEFT_OFFSET 0
#define TREE_RIGHT_OFFSET 1
#define TREE_HEIGHT_OFFSET 2
#define FB_DONE 1
#define FB_LEFT 2
#define FB_RIGHT 3

struct tree_node_t{
	unsigned left;  //0
	unsigned right;	//1
	int height;     //2
	int data;       //3
};

struct sub_t{
	int feedback;
	ptr_t pointer;
};

struct search_t{
	ptr_t nodePtr;
	ptr_t parentPtr;
	int direction;
	ptr_t stackPtr; // list stack
	ptr_t stackPtr_avl; // fixed stack
};
struct stack_t{
	ptr_t hdPtr; // list stack
	ptr_t pointer;
	int operation;
	ptr_t hdPtr_avl; // fixed stack
};

struct ptrBundle_t{
	ptr_t root;
	ptr_t nodePtr;
};

/* AVL tree extra functions */
int max(int a, int b);
int CalculateNodeHeight(data_t *Master2Mem, ptr_t nodePtr);
int height(data_t *Master2Mem, ptr_t nodePtr);
ptr_t RightRotate(data_t *Master2Mem, ptr_t yPtr);
ptr_t LeftRotate(data_t *Master2Mem, ptr_t xPtr);
signed GetBalance(data_t *Master2Mem, ptr_t nodePtr);
ptr_t ProcessNodeInsertion(data_t *Master2Mem, ptr_t nowPtr, int key);
ptr_t ProcessNodeDeletion(data_t *Master2Mem, ptr_t nowPtr);

void tree_node_set_height(data_t *Master2Mem, ptr_t currentNodePtr, int height);
void tree_node_write_height(data_t *Master2Mem, ptr_t nodePtr, data_t height);
data_t tree_node_read_height(data_t *Master2Mem, ptr_t nodePtr);

struct stack_t staticStack(int *myStack, int hdPtr_avl, int command, ptr_t pointer, int operation);


/* Base Functions */
/* Memory Node Access - Write Node */
void tree_node_write_left(data_t *Master2Mem, ptr_t nodePtr, next_t offset);
void tree_node_write_right(data_t *Master2Mem, ptr_t nodePtr, next_t offset);
void tree_node_write_data(data_t *Master2Mem, ptr_t nodePtr, data_t data);
void tree_node_write(data_t *Master2Mem, ptr_t nodePtr, struct tree_node_t currentNode);
/* Memory Node Access - Read Node */
next_t tree_node_read_left(data_t *Master2Mem, ptr_t nodePtr);
next_t tree_node_read_right(data_t *Master2Mem, ptr_t nodePtr);
data_t tree_node_read_data(data_t *Master2Mem, ptr_t nodePtr);
struct tree_node_t node_read(data_t *Master2Mem, ptr_t nodePtr);
/* Low-level tree manipulation functions */
void tree_node_set_left(data_t *Master2Mem, ptr_t currentNodePtr, ptr_t nextNodePtr);
ptr_t tree_node_alloc_new(data_t *Master2Mem, data_t *Master2SysAlloc, data_t data, ptr_t leftNodePtr, ptr_t rightNodePtr);
ptr_t tree_node_get_left_pointer(data_t *Master2Mem, ptr_t currentNodePtr);
ptr_t tree_node_get_right_pointer(data_t *Master2Mem, ptr_t currentNodePtr);
void tree_node_delete(data_t *Master2SysAlloc, ptr_t nodePtr);

/* Application Level functions */
/* Create Tree */
ptr_t TreeGen(data_t *Master2Mem, data_t *Master2SysAlloc, data_t *fixedStack, ptr_t stackPtr_avl, int NumberOfNodes);
struct ptrBundle_t Insert(data_t *Master2Mem, data_t *Master2SysAlloc, data_t *fixedStack, ptr_t stackPtr_avl, ptr_t treePtr, int data);
struct sub_t InsertSub(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t treePtr, data_t data);
/* Search */
struct search_t Search(data_t *Master2Mem, data_t *fixedStack, ptr_t stackPtr_avl, ptr_t treePtr, int data);
struct sub_t SearchSub(data_t *Master2Mem, ptr_t treePtr, int data);
/* Delete Tree */
ptr_t DeleteTree(data_t *Master2Mem, data_t *Master2SysAlloc, data_t *fixedStack, ptr_t stackPtr_avl, ptr_t treePtr);
struct sub_t DeleteTreeSub(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t treePtr);
/* Delete Node */
ptr_t DeleteTreeNode(data_t *Master2Mem, data_t *Master2SysAlloc,data_t *fixedStack, ptr_t stackPtr_avl, ptr_t rootPtr, int key);
/* Update Node */
ptr_t UpdateNode(data_t *Master2Mem, data_t *Master2SysAlloc, data_t *fixedStack, ptr_t stackPtr_avl, ptr_t rootPtr, int oldKey, int newKey);

/* Stack */
#define READ_STACK 0
#define WRITE_STACK 1
#define GOING_LEFT 0
#define GOING_RIGHT 1
struct stack_t dynamicStack(data_t *Master2Mem, data_t *Master2SysAlloc, ptr_t hdPtr, int command, ptr_t pointer, int operation);

#endif
