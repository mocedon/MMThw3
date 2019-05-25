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

// Private functions

int getSquareKey(pNode sq) {
	square* tmp = (square*)sq ;
	return tmp->key ;
}

pNode cloneSquare(pNode sq) {
	square* newSq = (square*)malloc(sizeof(square)) ;
	if (newSq == NULL) return NULL ;
	square* tmp = (square*)sq;
	newSq->BLX = tmp->BLX ;
	newSq->BLY = tmp->BLY ;
	newSq->dep = tmp->dep ;
	newSq->key = tmp->key ;
	return newSq ;
}

void printSquare(pNode sq) {
	square* tmp = (square*)sq;
	printf("[%.5lf , %.5lf]", tmp->BLX , tmp->BLY) ;
}

void delSquare(pNode sq) {
	free (sq) ;
}

square* newSquare(double x, double y, int l  , int k) {
	square* sq = (square*) malloc(sizeof(square)) ;
	double seg = (double) 1/l ;
	int prtX = (double) x / seg ;
	int prtY = (double) y / seg ;
	sq->BLX = (double) prtX * seg ;
	sq->BLY = (double) prtY * seg ;
	sq->dep = l ;
	sq->key = k ;
	return sq ;
}

int newKey(double x, double y, int oldKey) {
	square* sq = (square*)TreeGetNode(tree , oldKey)  ;
	int quarter = 1;
	double halfSeg = (double) 1 / (2 * sq->dep) ;
	double offSetX = x - sq->BLX ;
	double offSetY = y - sq->BLY ;
	free (sq) ;
	if ((offSetX / halfSeg) > 1) quarter++ ;
	if ((offSetY / halfSeg) > 1) quarter += 2 ;
	return (oldKey*10 + quarter) ;
}

// Public functions

void InitPartition() {
	tree = TreeCreate(getSquareKey , cloneSquare , printSquare , delSquare, QUARTER) ;
	square* mainSq = newSquare(0 , 0 , 1 , 0) ;
	TreeAddLeaf(tree , 0 , mainSq) ;
	free (mainSq) ;
}

void RefineCell(double x, double y) {
	// Case for non initiated main square
	int key = 0 ;
	int prvKey = 0 ;
	int depth = 1 ;
	int createSq = 0 ;
	while (!createSq) {
		Bool active = FALSE ;
		Result foundSq = TreeNodeIsActive(tree , key , &active) ;
		if (foundSq == FAILURE) {
			square* sq = newSquare(x , y , depth , key) ;
			TreeAddLeaf(tree , prvKey , sq) ;
			createSq = 1 ;
			free (sq) ;
		}
		if (foundSq == SUCCESS) {
			prvKey = key ;
			key = newKey(x , y , key) ;
			depth *= 2 ;
		}
	}

}

void PrintPartition() {
	TreePrint(tree) ;
	printf("\n") ;
}

void DeletePartition() {
	TreeDestroy(tree) ;
}

