// partition.c implementation
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "gentree.h"

#define QUARTER 4

pTree tree ;

/*

 node fuctions
typedef int(*GetKeyFunction)(pNode e);
typedef pNode(*CloneFunction)(pNode e);
typedef void(*PrintFunction)(pNode e);
typedef void(*DelFunction)(pNode e);

*/
// Set structure

typedef struct _square {
	double BLX , BLY ; // Bottom left X,Y
	int dep ;        // Segment length = 1/depth
	int key ;
}square;

// Private functions

int getSquareKey(pNode sq) {
	return sq->key ;
}

pNode cloneSquare(pNode sq) {
	square* newSq = malloc(sizeof(square)) ;
	if (newSq == NULL) return NULL ;
	newSq->BLX = sq->BLX ;
	newSq->BLY = sq->BLY ;
	newSq->dep = sq->dep ;
	newSq->key = sq->key ;
	return newSq ;
}

void printSquare(pNode sq) {
	printf("[%lf5 , lf5]", sq->BLX , sq->BLY)
}

void delSquare(pNode sq) {
	free (sq) ;
}

void newSquare(square* sq , double x, double y, int l  , int k) {
	sq->BLX = x ;
	sq->BLY = y ;
	sq->dep = l ;
	sq->key = k ;
}

int newKey(double x, double y, int oldKey) {
	square* sq = (square*)TreeGetNode(tree , oldKey)  ;
	int quarter = 1;
	double halfSeg = 1 / (2 * sq->dep) ;
	double offSetX = x - sq->BLX ;
	double offSetY = y - sq->BLY ;
	free (sq) ;
	if ((offSetX / halfSeg) > 1) quarter++ ;
	iff ((offSetY / halfSeg) > 1) quarter += 2 ;
	return (oldKey*10 + quarter) ;
}

// Public functions

void InitPartition() {
	tree = TreeCreate(getSquareKey , cloneSquare , printSquare , delSquare) ;
	square* mainSq ;
	newSquare(mainSq , 0 , 0 , 1 , 0) ;
	TreeAddLeaf(tree , 0 , mainSq) ;
}

void RefineCell(double x, double y) {
	// Case for non initiated main square
	int key = 0 ;
	int prvKey = 0;
	int depth = 1
	int createSq = 0 ;
	while (!createSq) {
		Bool active = FALSE ;
		Result foundSq = TreeNodeIsActive(tree , key , &active) ;
		if (foundSq == FAILURE) {
			square* sq ;
			newSquare(sq , x , y , depth , key)
			TreeAddLeaf(tree , prvKey , sq) ;
			createSq = 1 ;
		}
		if (foundSq == SUCCESS) {
			prvKey = key ;
			key = newKey(x , y , key) ;
			depth++ ;
		}
	}

}

void PrintPartition() {
	TreePrint(tree) ;
}

void DeletePartition() {
	TreeDestroy(tree) ;
}

