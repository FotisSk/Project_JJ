#ifndef grail__h__
#define grail__h__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include "definition.h"
#define STRLEN 1024
#define N 15
#define HASH 200

typedef struct hyperGraphStruct
{
	int size;
	int *strongNeighbors;
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

#endif	