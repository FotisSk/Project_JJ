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
#define N 10
#define HASH 200

typedef struct hyperGraphStruct
{
	//strongNeighborsIndex
	int indexSize;
	int *strongNeighborsIndex;
	
	//strongNeighbors
	int *strongNeighbors;
	int size;
	int nextAvailablePos;

	//grail
	int minRank;
	int rank;
	int parent;
	int unvisitedChildren;
		
}hyperGraphStruct;


typedef struct grailFront
{
	int* frontArray;
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

typedef struct grailIndex
{
	int index;
	int size;
	hyperGraphStruct *hyperGraph;
	grailFront *frontier;
}grailIndex;


hyperGraphStruct* createHyperGraph(Buffer* , NodeIndex* , SCC* );
void destroyHyperGraph(hyperGraphStruct* , int );

void pushFrontier(int , grailFront *);
int popFrontier(grailFront *);
int minRankOfChildren(int , hyperGraphStruct *);
int grailExpand(int , hyperGraphStruct *, grailFront *);

grailIndex* buildGrailIndex(NodeIndex *, Buffer *, SCC *);
void destroyGrailIndex(grailIndex *);
#endif	