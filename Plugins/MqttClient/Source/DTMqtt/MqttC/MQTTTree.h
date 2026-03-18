// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#if !defined(TREE_H)
#define TREE_H

#include <stdlib.h> /* for size_t definition */

/*BE
defm defTree(T) // macro to define a tree

def T concat Node
{
	n32 ptr T concat Node "parent"
	n32 ptr T concat Node "left"
	n32 ptr T concat Node "right"
	n32 ptr T id2str(T)
	n32 suppress "size"
}


def T concat MQTT_Tree
{
	struct
	{
		n32 ptr T concat Node suppress "root"
		n32 ptr DATA suppress "compare"
	} 
	struct
	{
		n32 ptr T concat Node suppress "root"
		n32 ptr DATA suppress "compare"
	} 
	n32 dec "count"
	n32 dec suppress "size"
}

endm

defTree(INT)
defTree(STRING)
defTree(TMP)

BE*/

/**
 * Structure to hold all data for one list element
 */
typedef struct MQTT_NodeStruct
{
	struct MQTT_NodeStruct *parent,   /**< pointer to parent tree node, in case we need it */
					  *child[2]; /**< pointers to child tree nodes 0 = left, 1 = right */
	void* content;				 /**< pointer to element content */
	size_t size;					 /**< size of content */
	unsigned int red : 1;
} Node;


/**
 * Structure to hold all data for one tree
 */
typedef struct
{
	struct
	{
		Node *root;	/**< root node pointer */
		int (*compare)(void*, void*, int); /**< comparison function */
	} index[2];
	int indexes,  /**< no of indexes into tree */
		count;    /**< no of items */
	size_t size;  /**< heap storage used */
	unsigned int heap_tracking : 1; /**< switch on heap tracking for this tree? */
	unsigned int allow_duplicates : 1; /**< switch to allow duplicate entries */
} MQTT_Tree;


MQTT_Tree* MQTTTreeInitialize(int(*compare)(void*, void*, int));
void MQTTTreeInitializeNoMalloc(MQTT_Tree* aTree, int(*compare)(void*, void*, int));
void MQTTTreeAddIndex(MQTT_Tree* aTree, int(*compare)(void*, void*, int));

void* MQTTTreeAdd(MQTT_Tree* aTree, void* content, size_t size);

void* MQTTTreeRemove(MQTT_Tree* aTree, void* content);

void* MQTTTreeRemoveKey(MQTT_Tree* aTree, void* key);
void* MQTTTreeRemoveKeyIndex(MQTT_Tree* aTree, void* key, int index);

void* MQTTTreeRemoveNodeIndex(MQTT_Tree* aTree, Node* aNode, int index);

void MQTTTreeFree(MQTT_Tree* aTree);

Node* MQTTTreeFind(MQTT_Tree* aTree, void* key);
Node* MQTTTreeFindIndex(MQTT_Tree* aTree, void* key, int index);

Node* MQTTTreeNextElement(MQTT_Tree* aTree, Node* curnode);

int MQTTTreeIntCompare(void* a, void* b, int);
int MQTTTreePtrCompare(void* a, void* b, int);
int MQTTTreeStringCompare(void* a, void* b, int);

#endif
