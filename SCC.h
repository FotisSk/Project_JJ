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

typedef struct Component
{
	uint32_t component_id; //current component id
	uint32_t included_nodes_count;	//number of nodes in component
	uint32_t* included_node_ids;	//ids of included nodes
}Component;

typedef struct SCC
{
	Component* components; // Components index - a vector which stores the components information
	uint32_t components_count;
	uint32_t *id_belongs_to_component;	//inverted index
}SCC;

typedef struct ComponentCursor
{
	Component* component_ptr; // pointer to current’s iteration component. Any other necessary information in order to move to next component in the vector
}ComponentCursor;


SCC* estimateStronglyConnectedComponents(NodeIndex* );
int findNodeStronglyConnectedComponentID(SCC* , uint32_t );
bool next_StronglyConnectedComponentID(SCC* , ComponentCursor* );
int estimateShortestPathStronglyConnectedComponents(SCC* , NodeIndex* , uint32_t , uint32_t );