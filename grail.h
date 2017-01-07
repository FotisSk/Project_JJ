#ifndef grail__h__
#define grail__h__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include "definition.h"
#include "SCC.h"
#define STRLEN 1024
#define N 1
#define HASH 200

typedef struct hyperGraphStruct
{
	//strongNeighborsIndex
	int indexSize;
	int *strongNeighborsIndex;
	
	//strongNeighbors
	int *strongNeighbors;
	int size;
	int nextAvailablePos;	//einai kai gia ton randIndex, parallila

	//grail
	//int minRank;
	//int rank;
	//int parent;
	//int unvisitedChildren;

	//int *randIndex;
		
}hyperGraphStruct;

typedef struct frontierEntry
{
	int sccID;
	int parent;
}frontierEntry;

typedef struct grailFront
{
	frontierEntry *frontArray;
	int size;
	int last;
}grailFront;

typedef struct grailStack
{
	int* stackArray;
	int size;
	int current;
	int last;
}grailStack;

typedef struct grailStruct
{
	int minRank;
	int rank;
	int parent;
	int unvisitedChildren;

	int *randIndex;
	int randIndexSize;
}grailStruct;

typedef struct grailIndex
{
	int index;
	int size;
	grailFront *frontier;
	grailStruct *grailArray;

}grailIndex;


hyperGraphStruct* createHyperGraph(Buffer* , NodeIndex* , SCC*);
void destroyHyperGraph(hyperGraphStruct* , int );

void pushFrontier(int , int, grailFront *);
frontierEntry popFrontier(grailFront *);
int minRankOfChildren(int , hyperGraphStruct *, grailStruct *);
int grailExpand(int , hyperGraphStruct *, grailFront *, grailStruct *);

grailIndex* buildGrailIndex(NodeIndex *, Buffer *, SCC *, hyperGraphStruct *);
int isReachableGrailIndex(grailIndex **, SCC *, int , int );
void destroyGrailIndex(grailIndex **);
#endif	