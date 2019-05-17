#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "defs.h"
#include "gentree.h"

/* definition of the element in the tree */
struct _ELEMENT
{
	pNode obj;
	PELEMENT* children;
	PELEMENT parent;
	int childrenCount;
}

/* definition of the tree structure */
struct _tree
{
	PELEMENT head;
	GetKeyFunction getKey;
	CloneFunction clone;
	PrintFunction print;
	DelFunction del;
	int k;
}

/* hidden fuctions */
void delElem(PELEMENT elem, DelFunction del)//aux of TreeDestroy
{
	for (int i = 0; i < elem->childrenCount; i++)
	{
		delElem(elem->children[i], del);
	}
	free(elem->children);
	del(elem->obj);
	free(elem);
}

int countElem(PELEMENT elem)//aux of TreeNodesCount
{
	int count = 1;
	for (int i = 0; i < elem->childrenCount; i++)
	{
		count += countElem(elem->children[i]);
	}
	return count;
}

void printElem(PELEMENT elem, PrintFunction print)//aux of TreePrint
{
	print(elem->obj);
	for (int i = 0; i < elem->childrenCount; i++)
	{
		printElem(elem->children[i], print);
	}
}

PELEMENT findElem(PELEMENT elem, int key, GetKeyFunction getKey)
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

PELEMENT createElem(pNode node, CloneFunction clone, int k)//creates new tree element
{
	PELEMENT elem = malloc(sizeof(ELEMENT));
	if (elem == NULL)
	{
		return NULL;
	}
	PELEMENT* children = malloc(sizeof(PELEMENT)*k);
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

PELEMENT copyElem(PELEMENT elem)
{
	if (elem == NULL)
	{
		return NULL;
	}
	PELEMENT newElem = malloc(sizeof(ELEMENT));
	if (newElem == NULL)
	{
		return NULL;
	}
	newElem->obj = elem->obj;
	newElem->children = elem->children;
	newElem->childrenCount = elem->childrenCount;
	newElem->parent = elem->parent;
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
	pTree newTree = malloc(sizeof(Tree));
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
	return nodeCount(t->head);
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
	answer* = FALSE;
	PELEMENT elem = findElem(t->head, key, t->getKey);
	if (elem == NULL)
	{
		return FAILURE;
	}
	if (elem->childrenCount < t->k)
	{
		answer* = TRUE;
	}
	return SUCCESS;
}

Result TreeNodeIsLeaf(pTree t, int key, Bool* answer)
{
	answer* = FALSE;
	PELEMENT elem = findElem(t->head, key, t->getKey);
	if (elem == NULL)
	{
		return FAILURE;
	}
	if (elem->childrenCount == 0)
	{
		answer* = TRUE;
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
	if (elem = t->head)
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
	return SUCCESS;
}

PELEMENT TreeGetRoot(pTree t)
{
	reutrn copyElem(t->head);
}

PELEMENT TreeGetNode(pTree t, int key)
{
	PELEMENT elem = findElem(t->head, key, t->getKey);
	return copyElem(elem);
}

PELEMENT* TreeGetChildren(pTree t, int key)
{
	PELEMENT elem = findElem(t->head, key, t->getKey);
	if (elem == NULL || elem->children == NULL)
	{
		return NULL;
	}
	PELEMENT* children = malloc(sizeof(PELEMENT)*(t->k));
	if (children == NULL)
	{
		return NULL;
	}
	for (int i = 0; i < t->k; i++)
	{
		children[i] = elem->children[i];
	}
	return children;
}