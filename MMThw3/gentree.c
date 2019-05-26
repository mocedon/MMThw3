#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "defs.h"
#include "gentree.h"

/* definition of the element in the tree */
typedef struct _ELEMENT
{
	pNode obj;
	struct _ELEMENT** children;
	struct _ELEMENT* parent;
	int childrenCount;
}ELEMENT;

typedef ELEMENT* PELEMENT;

/* definition of the tree structure */
typedef struct _tree
{
	PELEMENT head;
	GetKeyFunction getKey;
	CloneFunction clone;
	PrintFunction print;
	DelFunction del;
	int k;
}Tree;

/* hidden fuctions */
void delElem(PELEMENT elem, DelFunction del) //recurcively deletes an element and all decendands
{
	for (int i = 0; i < elem->childrenCount; i++)
	{
		delElem(elem->children[i], del);
	}
	free(elem->children);
	del(elem->obj);
	free(elem);
}

int countElem(PELEMENT elem) //recurcively counts number of decendants
{
	int count = 1;
	for (int i = 0; i < elem->childrenCount; i++)
	{
		count += countElem(elem->children[i]);
	}
	return count;
}

void printElem(PELEMENT elem, PrintFunction print) //recurcively prints decandants
{
	print(elem->obj);
	for (int i = 0; i < elem->childrenCount; i++)
	{
		printElem(elem->children[i], print);
	}
}

PELEMENT findElem(PELEMENT elem, int key, GetKeyFunction getKey) //recurcively finds an element
{
	if (getKey(elem->obj) == key)
	{
		return elem;
	}
	for (int i = 0; i < elem->childrenCount; i++)
	{
		PELEMENT temp = findElem(elem->children[i], key, getKey);
		if (temp != NULL)
		{
			return temp;
		}
	}
	return NULL;
}

PELEMENT createElem(pNode node, CloneFunction clone, int k) //creates a new tree element
{
	PELEMENT elem = (PELEMENT)malloc(sizeof(ELEMENT));
	if (elem == NULL)
	{
		return NULL;
	}
	PELEMENT* children = (PELEMENT*)malloc(sizeof(PELEMENT)*k);
	if (children == NULL)
	{
		free(elem);
		return NULL;
	}
	pNode obj = clone(node);
	if (obj == NULL)
	{
		free(elem);
		free(children);
		return NULL;
	}
	for (int i = 0; i < k; i++)
	{
		children[i] = NULL;
	}
	elem->obj = obj;
	elem->children = children;
	elem->childrenCount = 0;
	elem->parent = NULL;
	return elem;
}

/* public fuctions */
pTree TreeCreate(GetKeyFunction getKey, CloneFunction clone,
	PrintFunction print, DelFunction del, int k)
{
	if (k < 1)
	{
		return NULL;
	}
	pTree newTree = (pTree)malloc(sizeof(Tree));
	if (newTree == NULL)
	{
		return NULL;
	}
	newTree->head = NULL;
	newTree->getKey = getKey;
	newTree->clone = clone;
	newTree->print = print;
	newTree->del = del;
	newTree->k = k;
	return newTree;
}

void TreeDestroy(pTree t)
{
	delElem(t->head, t->del);
	free(t);
}

int TreeNodesCount(pTree t)
{
	return countElem(t->head);
}

void TreePrint(pTree t)
{
	printElem(t->head, t->print);
}

Result TreeAddLeaf(pTree t, int key, pNode newLeaf)
{
	PELEMENT elem = createElem(newLeaf, t->clone, t->k);
	if (elem == NULL)
	{
		return FAILURE;
	}
	if (t->head == NULL)
	{
		t->head = elem;
		return SUCCESS;
	}
	PELEMENT parent = findElem(t->head, key, t->getKey);
	if (parent == NULL || parent->childrenCount == t->k)
	{
		delElem(elem, t->del);
		return FAILURE;
	}
	elem->parent = parent;
	parent->children[parent->childrenCount++] = elem;
	return SUCCESS;
}

Result TreeNodeIsActive(pTree t, int key, Bool* answer)
{
	*answer = FALSE;
	PELEMENT elem = findElem(t->head, key, t->getKey);
	if (elem == NULL)
	{
		return FAILURE;
	}
	if (elem->childrenCount < t->k)
	{
		*answer = TRUE;
	}
	return SUCCESS;
}

Result TreeNodeIsLeaf(pTree t, int key, Bool* answer)
{
	*answer = FALSE;
	PELEMENT elem = findElem(t->head, key, t->getKey);
	if (elem == NULL)
	{
		return FAILURE;
	}
	if (elem->childrenCount == 0)
	{
		*answer = TRUE;
	}
	return SUCCESS;
}

Result TreeDelLeaf(pTree t, int key)
{
	PELEMENT elem = findElem(t->head, key, t->getKey);
	if (elem == NULL || elem->childrenCount > 0)
	{
		return FAILURE;
	}
	if (elem == t->head)
	{
		delElem(elem, t->del);
		t->head = NULL;
		return SUCCESS;
	}
	PELEMENT parent = elem->parent;
	int i = 0;
	while (parent->children[i] != elem)
	{
		i++;
	}
	delElem(elem, t->del);
	parent->childrenCount--;
	for (; i < parent->childrenCount; i++)
	{
		parent->children[i] = parent->children[i + 1];
	}
	parent->children[parent->childrenCount] = NULL;
	return SUCCESS;
}

pNode TreeGetRoot(pTree t)
{
	if (t->head == NULL || t->head->obj == NULL)
	{
		return NULL;
	}
	return t->clone(t->head->obj);
}

pNode TreeGetNode(pTree t, int key)
{
	PELEMENT elem = findElem(t->head, key, t->getKey);
	if (elem == NULL || elem->obj == NULL)
	{
		return NULL;
	}
	return t->clone(elem->obj);
}

pNode* TreeGetChildren(pTree t, int key)
{
	PELEMENT elem = findElem(t->head, key, t->getKey);
	if (elem == NULL || elem->children == NULL)
	{
		return NULL;
	}
	pNode* children = (pNode*)malloc(sizeof(pNode)*(t->k));
	if (children == NULL)
	{
		return NULL;
	}
	for (int i = 0; i < elem->childrenCount; i++)
	{
		children[i] = t->clone(elem->children[i]->obj);
		if (children[i] == NULL)
		{
			for (int j = 0; j < i; j++)
			{
				free(children[j]);
			}
			free(children);
			return NULL;
		}
	}
	for (int i = elem->childrenCount; i < t->k; i++)
	{
		children[i] = NULL;
	}
	return children;
}