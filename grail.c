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
#define N 10


//prosoxi sta uint32_t vs int aplo.
//dokimi periptosis pou na ginetai realloc ston pinaka strongNeighbors

//arxikopoihsh scc gia paradeigma diko mou.
hyperGraphStruct* createHyperGraph(Buffer* buffer, NodeIndex* index, SCC* sccPtr)
{
	int i, j, k, offset, sccID, nodeToCheck, flag, indexSize, nextAvailablePos;
	int *newstrongNeighborsIndex, *newStrongNeighbors;
	uint32_t *includedNodeIds;
	hyperGraphStruct *hyperGraphArray;
	Component *comp;
	indexStruct *index_array;
	list_node *buffer_array;

	//creation
	hyperGraphArray = malloc(sccPtr->components_count*sizeof(hyperGraphStruct));
	//initialization
	for(i=0; i<sccPtr->components_count; i++)
	{
		hyperGraphArray[i].strongNeighborsIndex = malloc(5*sizeof(int));
		hyperGraphArray[i].strongNeighbors = malloc(5*sizeof(int));
		for(j=0; j<5; j++)
		{
			hyperGraphArray[i].strongNeighborsIndex[j] = 0;
			hyperGraphArray[i].strongNeighbors[j] = -1;
		}
		hyperGraphArray[i].indexSize = 5;
		hyperGraphArray[i].size = 5;
		hyperGraphArray[i].nextAvailablePos = 0;
	}
	buffer_array = buffer -> buffer_array;
	index_array = index -> index_array;
	indexSize = index -> size;
	comp = sccPtr -> components;
	for(i=0; i<sccPtr->components_count; i++)	//gia kathe thesi tou pinaka components ->
	{
		includedNodeIds = comp[i].included_node_ids;
		for(j=0; j< comp[i].included_nodes_count; j++)	//gia kathe thesi  tou included_node_ids pou dn exei skoupidia ->
		{
			printf("\nj: %d\n", j);
			printf("node: %d\n", includedNodeIds[j]);
			if(includedNodeIds[j] >= indexSize)
			{
				printf("Node: %d doesn't have any neighbors (also out of bounds of the outgoing index array).\n", includedNodeIds[j]);
				continue;
			}
			offset = index_array[includedNodeIds[j]].offset;
			if(offset == -1)
			{
				printf("node: %d doesn't have any neighbors.\n", includedNodeIds[j]);
				continue;
			}
			//tora pou exeis to offset pigene ston buffer array kai elenkse olous tous geitones.
			int enp = buffer_array[offset].emptyNeighborPos;
			flag = 0;
			while(flag == 0)
			{
				printf("Mpika se Neighbor[] j: %d\n", j);
				for(k=0; k<enp; k++)
				{
					//printf("k: %d\n", k);
					nodeToCheck = buffer_array[offset].neighbor[k];
					sccID = sccPtr -> id_belongs_to_component[nodeToCheck];	//ara o k geitonas tou komvou sti thesi j tou included_node_ids anikei sti sscID isxira sinektiki sinistosa.
					if(sccID == comp[i].component_id)
					{
						printf("neighbor node: %d belongs to the same scc as node: %d\n", nodeToCheck, includedNodeIds[j]);
						continue;
					}

					printf("neighbor node: %d belongs to the scc: %d\n", nodeToCheck, sccID);
					//prota prepei na tsekaroume ton komvo se sxesi me to megethos tou strongNeighborsIndex[]. ton exo na leitourgei san hash (px 1h thesi: 0,1 analoga an exei geitona tin 1h sinistosa)
					if(sccID >= hyperGraphArray[i].indexSize)
					{
						printf("sccID: %d is bigger than arrays size. (realloc)\n", sccID);	
						while(sccID >= hyperGraphArray[i].indexSize)
						{
							hyperGraphArray[i].indexSize = 2*hyperGraphArray[i].indexSize;
						}
						newstrongNeighborsIndex = realloc(hyperGraphArray[i].strongNeighborsIndex, hyperGraphArray[i].indexSize * sizeof(int));
						hyperGraphArray[i].strongNeighborsIndex = newstrongNeighborsIndex;
						//stin periptosi auti einai sigouro oti den iparxei idi auti i scc sto pinakaki mas ton strongNeighborsIndex, opote pamekai ti vazoume
						hyperGraphArray[i].strongNeighborsIndex[sccID] = 1;
					    printf("Thesi ston hyperGraphArray: %d, scc: %d added as strongNeighbor of scc: %d.\n",i, sccID, comp[i].component_id);
					    nextAvailablePos = hyperGraphArray[i].nextAvailablePos;
					    if(nextAvailablePos >= hyperGraphArray[i].size)
					    {
					    	newStrongNeighbors = realloc(hyperGraphArray[i].strongNeighbors, hyperGraphArray[i].indexSize * sizeof(int));
					    	hyperGraphArray[i].strongNeighbors = newStrongNeighbors;
					    }
					    hyperGraphArray[i].strongNeighbors[nextAvailablePos] = sccID;
					    hyperGraphArray[i].nextAvailablePos++;

						continue;
					}
					
					if(hyperGraphArray[i].strongNeighborsIndex[sccID] == 0)
					{
						hyperGraphArray[i].strongNeighborsIndex[sccID] = 1;
						printf("Thesi ston hyperGraphArray: %d, scc: %d added as strongNeighbor of scc: %d.\n",i, sccID, comp[i].component_id);
						nextAvailablePos = hyperGraphArray[i].nextAvailablePos;
					    if(nextAvailablePos >= hyperGraphArray[i].size)
					    {
					    	newStrongNeighbors = realloc(hyperGraphArray[i].strongNeighbors, hyperGraphArray[i].indexSize * sizeof(int));
					    	hyperGraphArray[i].strongNeighbors = newStrongNeighbors;
					    }
					    hyperGraphArray[i].strongNeighbors[nextAvailablePos] = sccID;
					    hyperGraphArray[i].nextAvailablePos++;
					}
					else
						printf("Thesi ston hyperGraphArray: %d, strongNeighbor %d already exists.\n",i, sccID);
				}//for k
				if(buffer_array[offset].nextListNode == -1)
					flag = 1;
				else
				{
					offset = buffer_array[offset].nextListNode;
					enp = buffer_array[offset].emptyNeighborPos;
				}
			}//while flag
		}//for j
	}//for i
	return hyperGraphArray;
}