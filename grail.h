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
	int indexSize;
	int *strongNeighborsIndex;
	
	int *strongNeighbors;
	int size;
	int nextAvailablePos; //einai gia ton strongNeighbors
		//einai gia ton strongNeighbors
}hyperGraphStruct;


typedef struct grailFront
{
	int* front_array;
	int size;
	int last;
}grailFront;

typedef struct grailStack
{
	int* stack_array;
	int size;
	int current;
	int last;
}grailStack;


hyperGraphStruct* createHyperGraph(Buffer* , NodeIndex* , SCC* );
#endif	