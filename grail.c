#include "grail.h"
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


//arxikopoihsh scc gia paradeigma diko mou.
hyperGraphStruct* createHyperGraph(Buffer* buffer, NodeIndex* index, SCC* sccPtr)
{
	int i, j, k, offset, sccID, nodeToCheck;
	int *newStrongNeighbors;
	uint32_t *includedNodeIds;
	hyperGraphStruct *hyperGraphIndex;
	Component *comp;
	indexStruct *index_array;
	list_node *buffer_array;

	//creation
	hyperGraphIndex = malloc(sccPtr->components_count*sizeof(hyperGraphStruct));
	//initialization
	for(i=0; i<sccPtr->components_count; i++)
	{
		hyperGraphIndex[i].strongNeighbors = malloc(50*sizeof(int));
		for(j=0; j<50; j++)
		{
			hyperGraphIndex[i].strongNeighbors[j] = 0;
		}
		hyperGraphIndex[i].size = 50;
	}

	buffer -> buffer_array = buffer_array;
	index -> index_array = index_array;
	comp = sccPtr -> components;
	for(i=0; i<sccPtr->components_count; i++)	//gia kathe thesi tou pinaka components ->
	{
		for(j=0; j< comp[i].included_nodes_count; j++)	//gia kathe thesi  tou included_node_ids pou dn exei skoupidia ->
		{
			includedNodeIds = comp[i].included_node_ids;
			offset = index_array[ includedNodeIds[j] ].offset;
			//tora pou exeis to offset pigene ston buffer array kai elenkse olous tous geitones.
			for(k=0; k<N; k++)
			{
				nodeToCheck = buffer_array[offset].neighbor[k];
				sccID = sccPtr -> id_belongs_to_component[nodeToCheck];	//ara o k geitonas tou komvou sti thesi j tou included_node_ids anikei sti sscID isxira sinektiki sinistosa.

				//prota prepei na tsekaroume ton komvo se sxesi me to megethos tou strongNeighbors[]. ton exo na leitourgei san hash (px 1h thesi: 0,1 analoga an exei geitona tin 1h sinistosa)
				if(sccID >= hyperGraphIndex[i].size)
				{
					printf("sccID is bigger than arrays size. realloc.\n");	
					while(sccID >= hyperGraphIndex[i].size)
					{
						hyperGraphIndex[i].size = 2*hyperGraphIndex[i].size;
					}
					newStrongNeighbors = realloc(hyperGraphIndex[i].strongNeighbors, hyperGraphIndex[i].size * sizeof(hyperGraphStruct));
					hyperGraphIndex[i].strongNeighbors = newStrongNeighbors;
					//stin periptosi auti einai sigouro oti den iparxei idi auti i scc sto pinakaki mas ton strongNeighbors, opote pamekai ti vazoume
					hyperGraphIndex[i].strongNeighbors[sccID] = 1;
					continue;
				}
				
				if(hyperGraphIndex[i].strongNeighbors[sccID] == 0)
					hyperGraphIndex[i].strongNeighbors[sccID] = 1;
				else
					printf("This strongNeighbor already exists.\n");
			}
		}
	}
}