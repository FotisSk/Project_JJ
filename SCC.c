#include "SCC.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#define STRLEN 1024

SCC* SCC_create()
{
	SCC* scc;

	scc = malloc(sizeof(SCC));
	return scc;
}


void SCC_initialize(SCC* SCC_ptr, int array_length, NodeIndex* index)
{
	int i;
	int indexSize;
	int *invertedIndex;
	Component *componentIndex;

	componentIndex = malloc(sizeof(Component)*array_length);
	SCC_ptr -> components = componentIndex;

	indexSize = index -> size;
	invertedIndex = malloc( sizeof(int) * indexSize );
	for( i=0; i<indexSize; i++ ){
		invertedIndex[i] = -1;
	}
	SCC_ptr -> id_belongs_to_component = invertedIndex;
	SCC_ptr -> components_count = 0;
	
}