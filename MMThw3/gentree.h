#ifndef TREE_H
#define TREE_H

#include "defs.h"

/* declaring structs */
typedef struct _tree Tree;
typedef struct _ELEMENT ELEMENT;

/* naming pointers */
typedef Tree* pTree;
typedef ELEMENT* PELEMENT;
typedef void* pNode;

/* node fuctions */
typedef int(*GetKeyFunction)(pNode e);
typedef pNode(*CloneFunction)(pNode e);
typedef void(*PrintFunction)(pNode e);
typedef void(*DelFunction)(pNode e);

/* tree fuctions */
pTree TreeCreate(GetKeyFunction getKey, CloneFunction clone,
	PrintFunction print, DelFunction del, int k);

void TreeDestroy(pTree t);

int TreeNodesCount(pTree t);

void TreePrint(pTree t);

Result TreeAddLeaf(pTree t, int key, pNode newLeaf);

Result TreeNodeIsActive(pTree t, int key, Bool* answer);

Result TreeNodeIsLeaf(pTree t, int key, Bool* answer);

Result TreeDelLeaf(pTree t, int key);

PELEMENT TreeGetRoot(pTree t);

PELEMENT TreeGetNode(pTree t, int key);

PELEMENT* TreeGetChildren(pTree t, int key);

#endif
