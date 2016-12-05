#include "SCC.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#define STRLEN 1024
#define FRONTLEN 100


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
	SCC_ptr->size=array_length;
	SCC_ptr -> components = componentIndex;

	indexSize = index -> size;
	invertedIndex = malloc( sizeof(int) * indexSize );
	for( i=0; i<indexSize; i++ ){
		invertedIndex[i] = -1;
	}
	SCC_ptr -> id_belongs_to_component = invertedIndex;
	SCC_ptr -> components_count = 0;
	
}

Front* front_create()
{
	Front* front;
	front = malloc( sizeof(Front) );
	front->front_array=malloc( FRONTLEN*sizeof(int) );
	front->last=0;
	return front;
}

Stack* stack_create()
{
	Stack* stack;
	stack = malloc( sizeof(Front) );
	stack->stack_array=malloc( FRONTLEN*sizeof(int) );
	stack->last=0;
	stack->current=0;
	return stack;
}

void tarjan(NodeIndex* index, NodeIndex* index2, Buffer* buffer2, Buffer* buffer,SCC* scc)
{
	indexStruct* index_array=index->index_array;
	list_node* buffer_array=buffer->buffer_array;
	indexStruct* index_array2=index2->index_array;
	list_node* buffer_array2=buffer2->buffer_array;

	int Index=1;
	int i=0;
	int offset,offset2,comp_count;
//	comp_count=scc->components_count;
//	int size=index_array->size;
	Front* front=front_create();
	Stack* stack=stack_create();

	for(i=0 ; i < index->size ; i++)
	{
		comp_count=scc->components_count;
		offset=index_array[i].offset;
		offset2=index_array2[i].offset;

		if( (offset==-1)&&(offset2==-1) )
		{
			printf("Den uparxei o komvos %d! \n",i);
			continue;
		}

		// printf("Geitones tou %d : %d \n",i,buffer_array[offset].emptyNeighborPos);
		else if( (offset==-1) && (offset2 != -1) )
		{
			if(scc->components_count==scc->size)
			{
				scc->size=2*scc->size;
				scc->components=realloc(scc->components,scc->size);
				if(scc->components==NULL)
				{
					printf("Null realloc \n");
					return ;
				}
			}
		//	printf("O %d den exei geitones.Einai sunistwsa monh ths!\n",i);
			scc->components[comp_count].component_id=comp_count;
			scc->components[comp_count].included_nodes_count=1;
			scc->components[comp_count].included_node_ids=malloc( sizeof(int) );
			if(scc->components[comp_count].included_node_ids==NULL)
			{
			
			//	printf("Gia ton poutso PC sxolhs \n");
				return ;
			}
			scc->components[comp_count].included_node_ids[0]=i;

			scc->id_belongs_to_component[i]=comp_count;	 		
			scc->components_count++;

		}
	}
	return;
}