// partition.c implementation
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "gentree.h"

#define QUARTER 4

pTree tree ;
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

pNode cloneSquare(pNode sq
	{
	pSquare newSq = (pSquare)malloc(sizeof(square)) ;
	if (newSq == NULL) return NULL ;
	pSquare tmp = (pSquare)sq;
	newSq->BLX = tmp->BLX ;
	newSq->BLY = tmp->BLY ;
	newSq->dep = tmp->dep ;
	newSq->key = tmp->key ;
	return newSq ;
}

void printSquare(pNode sq)
{
	pSquare tmp = (pSquare)sq;
	printOneSquare(tmp);
	pSquare* in_sq = (pSquare*)TreeGetChildren(tree, tmp->key);
	for (int i = 0; i < QUARTER; i++)
	{
		if (in_sq[i] == NULL)
		{
			return;
		}
		printf("\\");
		printOneSquare(in_sq[i]);
	}
}

void delSquare(pNode sq)
{
	free (sq) ;
}

pSquare newSquare(double x, double y, int l  , int k)
{
	pSquare sq = (pSquare) malloc(sizeof(square)) ;
	double seg = (double) 1/l ;
	int prtX = (int) x / seg ;
	int prtY = (int) y / seg ;
	sq->BLX = (double) prtX * seg ;
	sq->BLY = (double) prtY * seg ;
	sq->dep = l ;
	sq->key = k ;
	return sq ;
}

int newKey(double x, double y, int oldKey)
{
	pSquare sq = (pSquare)TreeGetNode(tree , oldKey)  ;
	int quarter = 1;
	double halfSeg = (double) 1 / (2 * sq->dep) ;
	double offSetX = x - sq->BLX ;
	double offSetY = y - sq->BLY ;
	free (sq) ;
	if ((offSetX / halfSeg) > 1) quarter++ ;
	if ((offSetY / halfSeg) > 1) quarter += 2 ;
	return (oldKey*10 + quarter) ;
}

void printOneSquare(pSquare sq)
{
	double x = sq->BLX;
	double y = sq->BLY;
	double l = 1 / (double)sq->dep;
	printf("([%f, %f], [%f, %f])", x, y, x + l, y + l);
}

// Public functions

void InitPartition()
{
	tree = TreeCreate(getSquareKey , cloneSquare , printSquare , delSquare, QUARTER) ;
	pSquare mainSq = newSquare(0 , 0 , 1 , 0) ;
	TreeAddLeaf(tree , 0 , mainSq) ;
	free (mainSq) ;
}

void RefineCell(double x, double y)
{
	// Case for non initiated main square
	int key = 0 ;
	int prvKey = 0 ;
	int depth = 1 ;
	while (1)
	{
		Bool active = FALSE;
		Result foundSq = TreeNodeIsActive(tree, key, &active);
		if (foundSq == FAILURE)
		{
			pSquare sq = newSquare(x, y, depth, key);
			TreeAddLeaf(tree, prvKey, sq);
			free(sq);
			return;
		}
		prvKey = key;
		key = newKey(x, y, key);
		depth *= 2;
	}
}

void PrintPartition()
{
	TreePrint(tree) ;
	printf("\n") ;
}

void DeletePartition()
{
	TreeDestroy(tree) ;
}

