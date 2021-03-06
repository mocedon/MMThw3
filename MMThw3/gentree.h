#ifndef TREE_H
#define TREE_H

#include "defs.h"

typedef void* pNode;
typedef struct _tree* pTree;

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

pNode TreeGetRoot(pTree t);

pNode TreeGetNode(pTree t, int key);

pNode* TreeGetChildren(pTree t, int key);

#endif
