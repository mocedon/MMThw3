// partition.c implementation
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "gentree.h"

#define QUARTER 4

static pTree tree = NULL;

// Set structure
typedef struct _square {
	double BLX , BLY ; // Bottom left X,Y
	int dep ;          // Segment length = 1/dep
	int key ;
}square;

typedef square* pSquare;

// Private functions

int getSquareKey(pNode sq)
{
	pSquare tmp = (pSquare)sq ;
	return tmp->key ;
}

pNode cloneSquare(pNode sq)
{
	pSquare newSq = (pSquare)malloc(sizeof(square));
	if (newSq == NULL)
	{
		return NULL;
	}
	pSquare tmp = (pSquare)sq;
	newSq->BLX = tmp->BLX ;
	newSq->BLY = tmp->BLY ;
	newSq->dep = tmp->dep ;
	newSq->key = tmp->key ;
	return newSq ;
}

double getLength(pSquare sq)
{
	double dep = (double)sq->dep;
	return 1 / dep;
}

void printOneSquare(pSquare sq)
{
	double x = sq->BLX;
	double y = sq->BLY;
	double l = getLength(sq);
	printf("([%f, %f], [%f, %f])", x, x + l, y, y + l);
}

void printSquare(pNode sq)
{
	pSquare tmp = (pSquare)sq;
	printOneSquare(tmp);
	pSquare* in_sq = (pSquare*)TreeGetChildren(tree, tmp->key);
	if (in_sq == NULL)
	{
		return;
	}
	for (int i = 0; i < QUARTER; i++)
	{
		if (in_sq[i] == NULL)
		{
			break;
		}
		printf("\\");
		printOneSquare(in_sq[i]);
	}
	printf("\n");
	free(in_sq);
}

void delSquare(pNode sq)
{
	free(sq);
}

pSquare newSquare(double x, double y, int l  , int k)
{
	pSquare sq = (pSquare)malloc(sizeof(square));
	if (sq == NULL)
	{
		return NULL;
	}
	int prtX = x * l;
	int prtY = y * l;
	sq->BLX = (double)prtX / l;
	sq->BLY = (double)prtY / l;
	sq->dep = l;
	sq->key = k;
	return sq;
}

int newKey(pSquare sq, double x, double y)
{
	int quarter = 1;
	double halfSeg = getLength(sq) / 2;
	double offSetX = x - sq->BLX;
	double offSetY = y - sq->BLY;
	if (offSetX >= halfSeg)
	{
		quarter++;
	}
	if (offSetY >= halfSeg)
	{
		quarter += 2;
	}
	return ((sq->key) * QUARTER + quarter);
}

// Public functions

void InitPartition()
{
	if (tree != NULL)
	{
		TreeDestroy(tree);
	}
	tree = TreeCreate(getSquareKey , cloneSquare , printSquare , delSquare, QUARTER) ;
	if (tree == NULL)
	{
		exit(1);
	}
	pSquare mainSq = newSquare(0, 0, 1, 0);
	if (mainSq == NULL)
	{
		TreeDestroy(tree);
		exit(1);
	}
	if (TreeAddLeaf(tree, 0, mainSq) == FAILURE)
	{
		TreeDestroy(tree);
		free(mainSq);
		exit(1);
	}
	free(mainSq);
}

void RefineCell(double x, double y)
{
	// Case for non initiated main square
	if (x < 0 || y < 0 || x >= 1 || y >= 1)
	{
		return;
	}
	int key = 0 ;
	int prvKey = 0 ;
	int depth = 1 ;
	while (1)
	{
		pSquare sq = TreeGetNode(tree, key);
		if (sq == NULL)
		{
			sq = newSquare(x, y, depth, key);
			TreeAddLeaf(tree, prvKey, sq);
			free(sq);
			return;
		}
		prvKey = key;
		key = newKey(sq, x, y);
		depth *= 2;
		free(sq);
	}
}

void PrintPartition()
{
	TreePrint(tree);
}

void DeletePartition()
{
	TreeDestroy(tree);
}

