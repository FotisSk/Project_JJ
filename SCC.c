#include "SCC.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#define STRLEN 1024
#define FRONTLEN 5
#define EDGELEN 3
#define N 10

/*****************************************************************************************/
/******************************** CREATE AND INITIALIZE **********************************/
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

	for(i=0; i<array_length; i++)
	{
		SCC_ptr -> components[i].component_id = -1;
		SCC_ptr -> components[i].included_nodes_count = 0;
		SCC_ptr -> components[i].included_node_ids = malloc(array_length * sizeof(int));
	}

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
	front->size=FRONTLEN;
	front->last=-1;
	return front;
}

Stack* stack_create()
{
	Stack* stack;
	stack = malloc( sizeof(Stack) );
	stack->stack_array=malloc( FRONTLEN*sizeof(int) );
	stack->size=FRONTLEN;
	stack->last=-1;
	stack->current=0;
	return stack;
}


/*****************************************************************************************/
/************************************** FRONTIER *****************************************/

void push_frontier_tarjan(int node, Front* frontier, NodeIndex *index)
{
	if ((frontier->last+1) < frontier->size) 
	{
		frontier->last++;
		frontier->front_array[frontier->last]=node;

	}
	else 
	{
		frontier->size = frontier->size * 2;
		frontier->front_array = realloc(frontier->front_array, frontier->size * sizeof(int));
		frontier->last++;
		frontier->front_array[frontier->last]=node;
	}
	index -> index_array[node].inFrontier = 1;
}


int pop_frontier_tarjan(Front* frontier, NodeIndex* index) 
{

	if ( frontier->last == -1 ) 
	{
		printf("Adeia lista manmu \n");
		return -1;
	}
	else 
	{
		index -> index_array[frontier -> front_array[frontier->last]].inFrontier = 0;
		frontier->last --;	
		return frontier->front_array[ (frontier->last) +1];
	}

}


/*****************************************************************************************/
/*************************************** STACK *******************************************/

void push_stack_tarjan(int node, Stack* stack, NodeIndex* index)
{
	if ((stack->last+1) < (stack->size) ) 
	{
		stack->last++;
		stack->stack_array[stack->last]=node;

	}
	else 
	{
		stack->size = stack->size * 2;
		stack->stack_array = realloc(stack->stack_array, stack->size * sizeof(int));
		stack->last++;
		stack->stack_array[stack->last]=node;
	}
	index -> index_array[node].inStack = 1;
}


int pop_stack_tarjan(Stack* stack, NodeIndex* index) 
{
	if ( stack->last == -1 ) 
	{
		printf("Adeia lista manmu \n");
		return -1;
	}
	else 
	{
		index -> index_array[stack->stack_array[ stack->last ]].inStack = 0;
		stack->last --;	
		return stack->stack_array[ (stack->last) + 1 ];
	}
}

int expand_tarjan(int node,Front* frontier,NodeIndex* index, Buffer* buffer)
{
	int expandedNodes=0;
	int i,neighbor,nextNode;
	indexStruct* index_array=index->index_array;
	list_node* buffer_array=buffer->buffer_array;
	int offset=index_array[node].offset;
	nextNode=buffer_array[offset].nextListNode;

	while( nextNode != -1 )
	{
		for(i=0 ; i < buffer_array[offset].emptyNeighborPos ; i++ )
		{	

			neighbor=buffer_array[offset].neighbor[i];
			if( index_array[neighbor].tarjanvisited == 1 )
				continue;
			else
			{
				expandedNodes++;
				push_frontier_tarjan(neighbor ,frontier, index);
			}
		}
		offset=nextNode;
		nextNode=buffer_array[offset].nextListNode;
	}

	int tail=index_array[node].tail;
	for(i=0 ; i < buffer_array[tail].emptyNeighborPos ; i++ )
	{
		neighbor=buffer_array[tail].neighbor[i];
		if( index_array[neighbor].tarjanvisited == 1 )
			continue;
		else
		{
			expandedNodes++ ;
			push_frontier_tarjan(neighbor ,frontier, index);

		}
	}
	index_array[node].tarjanvisited=1;
	return expandedNodes;

}

void tarjan(NodeIndex* index, NodeIndex* index2, Buffer* buffer, Buffer* buffer2,SCC* scc)
{
	indexStruct* index_array=index->index_array;
	list_node* buffer_array=buffer->buffer_array;
	indexStruct* index_array2=index2->index_array;
	list_node* buffer_array2=buffer2->buffer_array;

	int Index=1;
	int i=0;
	int offset,offset2,comp_count, node, expandedNodes;
//	comp_count=scc->components_count;
//	int size=index_array->size;
	Front* frontier=front_create();
	Stack* stack=stack_create();

	for(i=0 ; i < index->size ; i++)
	{
		if(index_array[i].tarjanvisited == 1)
		{
			continue;
		}

		comp_count=scc->components_count;
		offset=index_array[i].offset;
		offset2=index_array2[i].offset;


		if( (offset==-1)&&(offset2==-1) )
		{
			//printf("Den uparxei o komvos %d! \n",i);
			continue;
		}

		// printf("Geitones tou %d : %d \n",i,buffer_array[offset].emptyNeighborPos);
		else if( (offset==-1) && (offset2 != -1) )
		{

			SCC_update_solo(scc, i);
			index_array[i].tarjanvisited=1;

		}
		else if ( offset != -1 )
		{
			//continue;
			//printf("Ulopoieitai tora\n");
			push_frontier_tarjan(i,frontier, index);
			while( frontier -> last != -1 )
			{
				if( frontier -> last ==  -1)
				{
					printf("Frontier is Empty. Something went wrong.\n");
					return;
				}
				node = pop_frontier_tarjan(frontier, index);
				if( index_array[node].lowlink == -1 || index_array[node].index == -1 )
				{
					index_array[node].lowlink = Index;
					index_array[node].index = Index;
					Index++;
				}
				expandedNodes = expand_tarjan(node, frontier, index, buffer);
				push_stack_tarjan(node, stack, index);

				if( expandedNodes == 0 )
				{
					printf("O komvos | %d | einai monos tou isxira sinektiki sinistosa\n", node);
					pop_stack_tarjan(stack, index);
					SCC_update_solo(scc, node);

				}
			}
		}
	}
	return;
}

void SCC_update_solo(SCC *scc, int node)
{
	int comp_count;

	comp_count = scc->components_count;
	if(comp_count == scc->size)
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
	scc->components[comp_count].component_id = comp_count;
	scc->components[comp_count].included_nodes_count = 1;
	scc->components[comp_count].included_node_ids = malloc( sizeof(int) );
	if(scc->components[comp_count].included_node_ids == NULL)
	{
		//	printf("Gia ton poutso PC sxolhs \n");
		return ;
	}
	scc->components[comp_count].included_node_ids[0] = node;

	scc->id_belongs_to_component[node]=comp_count;	 		
	scc->components_count++;
}




QueryComp* edge_table(QueryComp* ptr)
{

	int i;
	ptr=malloc(sizeof(QueryComp));
	ptr->table=malloc(EDGELEN*sizeof(int*));

	for(i=0 ; i < EDGELEN ; i++)
	{
		ptr->table[i]=malloc( 2*sizeof(int) );
	}
	ptr->size=EDGELEN;
	ptr->current=-1;
	return ptr;
}

void add_comp_edge(int cc1, int cc2, QueryComp* ptr)
{
	int pos;
	if( ptr->current == (ptr->size)-1 ) 
	{

//		printf("Tha kanei realloc kapote \n");
		ptr->size=(2*ptr->size);
//		printf("SIZEEEE 1 | %d |.",ptr->size);

		ptr->table=realloc(ptr->table , (ptr->size) * sizeof(int*) );
		for( pos=ptr->current+1 ; pos < (ptr->size) ; pos++)
		{
			ptr->table[pos]=malloc( 2*sizeof(int) );
		}
//		printf("Kanw relloc xazouli \n");
	}
//	printf("Klaiw \n");
//	printf("SIZEEEE 2 | %d |.",(ptr->size) ) ;
	ptr->table[(ptr->current)+1][0]=cc1;
	ptr->table[(ptr->current)+1][1]=cc2;
	ptr->current=ptr->current+1;
//	print_edge_table(ptr);

}

void print_edge_table(QueryComp* ptr)
{
	int i,length;
	length=ptr->size;
	int** table=ptr->table;
	printf("|==================================================| \n");
	for(i=0; i< ptr->current ; i++)
	{
		printf("Array[%d][0]= | %d | \n",i,table[i][0]);
		printf("Array[%d][1]= | %d | \n ",i,table[i][1]);
		printf("\n");
	}
	printf("|==================================================| \n");
}


void weekly(int node1, int node2, NodeIndex* index, NodeIndex* index2,int* new_component, QueryComp* ptr)
{
//	printf("CC1: | %d | . CC2 | %d | \n",index_array[node1].cc, index_array2[node2].cc);
//	printf("Node1:| %d | Node2: | %d | \n",node1,node2);
	//printf("CC1: | %d | . CC2 | %d | \n",index_array[node1].cc, index_array2[node2].cc);

	indexStruct* index_array=index->index_array;
	indexStruct* index_array2=index2->index_array;
	
//	printf("CC1: | %d | . CC2 | %d | \n",index_array[node1].cc, index_array2[node2].cc);

	//printf("Molis arxikopoihsa \n");
	if( (index_array[node1].cc == -1)&&(index_array2[node2].cc == -1 ) ) 
	{
//		printf("1 \n");
		index_array[node1].cc=(*new_component);
		index_array2[node2].cc=(*new_component);
		(*new_component)++;

	}
	else if ( (index_array[node1].cc != -1)&&(index_array2[node2].cc == -1) )
	{
//		printf("2 \n");
		index_array[node1].cc=index_array2[node2].cc;		
	}
	else if ( (index_array[node1].cc == -1)&&(index_array2[node2].cc != -1) )
	{
//		printf("3 \n");
		index_array2[node2].cc=index_array[node1].cc;
	}
	else if ( (index_array[node1].cc != -1)&&(index_array2[node2].cc != -1) )
	{
		if(index_array[node1].cc == index_array2[node2].cc)
		{
			return;
		}
//		printf("Paw na addarw sto pinakaki \n");
		add_comp_edge( index_array[node1].cc , index_array2[node2].cc, ptr);
	}
}

