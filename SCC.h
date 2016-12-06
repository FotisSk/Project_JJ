#ifndef SCC__H__
#define SCC__H__

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

typedef struct QueryComp
{
	int** table;
	int size;
	int current;
}QueryComp;

typedef struct Component
{
	uint32_t component_id; //current component id
	uint32_t included_nodes_count;	//number of nodes in component
	uint32_t* included_node_ids;	//ids of included nodes
}Component;

typedef struct SCC
{
	Component* components; // Components index - a vector which stores the components information
	int size;
	uint32_t components_count;
	uint32_t *id_belongs_to_component;	//inverted index
}SCC;

typedef struct Front
{
	int* front_array;
	int size;
	int last;
}Front;

typedef struct Stack
{
	int* stack_array;
	int size;
	int current;
	int last;
}Stack;


typedef struct ComponentCursor
{
	Component* component_ptr; // pointer to current’s iteration component. Any other necessary information in order to move to next component in the vector
}ComponentCursor;

Front* front_create();
Stack* stack_create();
SCC* estimateStronglyConnectedComponents(NodeIndex* );
int findNodeStronglyConnectedComponentID(SCC* , uint32_t );
bool next_StronglyConnectedComponentID(SCC* , ComponentCursor* );
int estimateShortestPathStronglyConnectedComponents(SCC* , NodeIndex* , uint32_t , uint32_t );
SCC* SCC_create();
void SCC_initialize(SCC* , int , NodeIndex* );

void tarjan(NodeIndex*, NodeIndex* , Buffer* , Buffer* ,SCC*);

void push_frontier_tarjan(int , Front* );
int pop_frontier_tarjan(Front*);
int expand_tarjan(int ,Front*,NodeIndex*, Buffer*);

QueryComp* edge_table(QueryComp*);
void add_comp_edge(int , int , QueryComp* );
void weekly(int , int , NodeIndex* , NodeIndex*,int* , QueryComp* );
void print_edge_table(QueryComp*);

#endif
