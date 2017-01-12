#include "grail.h"
#include "SCC.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <time.h>
#define STRLEN 1024
#define FRONTLEN 5
#define EDGELEN 3
#define numOfGrails 3
#define N 1


//prosoxi sta uint32_t vs int aplo.
//arxikopoihsh scc gia paradeigma diko mou.


/**********************************************************************************************************/
/********************************************** HYPERGRAPH ***********************************************/
hyperGraphStruct* createHyperGraph(Buffer* buffer, NodeIndex* index, SCC* sccPtr)
{
	int i, j, k, offset, sccID, nodeToCheck, flag, indexSize, nextAvailablePos;
	int *newstrongNeighborsIndex, *newStrongNeighbors, *newRandIndex;
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
		hyperGraphArray[i].strongNeighborsIndex = malloc(1*sizeof(int));
		hyperGraphArray[i].strongNeighbors = malloc(1*sizeof(int));
		//hyperGraphArray[i].randIndex = malloc(1*sizeof(int));
		for(j=0; j<1; j++)
		{
			hyperGraphArray[i].strongNeighborsIndex[j] = 0;
			//hyperGraphArray[i].strongNeighbors[j] = -1;
			//hyperGraphArray[i].randIndex[j] = 0;	
		}
		hyperGraphArray[i].indexSize = 1;
		hyperGraphArray[i].size = 1;
		hyperGraphArray[i].nextAvailablePos = 0;

		//grail
		//hyperGraphArray[i].minRank = 0;
		//hyperGraphArray[i].rank = 0;
		//hyperGraphArray[i].parent = -1;
		//hyperGraphArray[i].unvisitedChildren = -1;

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
				//printf("Mpika se Neighbor[] j: %d\n", j);
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
					    printf("hyperGraphArray[%d] -> scc: %d added as strongNeighbor.\n",i, sccID);
					    nextAvailablePos = hyperGraphArray[i].nextAvailablePos;
					    if(nextAvailablePos >= hyperGraphArray[i].size)
					    {
					    	printf("strongNeighbors is full (realloc)\n");
					    	hyperGraphArray[i].size = 2*hyperGraphArray[i].size;

					    	newStrongNeighbors = realloc(hyperGraphArray[i].strongNeighbors, hyperGraphArray[i].size * sizeof(int));
					    	hyperGraphArray[i].strongNeighbors = newStrongNeighbors;

					    	//newRandIndex = realloc(hyperGraphArray[i].randIndex, hyperGraphArray[i].size * sizeof(int));
					    	//hyperGraphArray[i].randIndex = newRandIndex;
					    }
					    hyperGraphArray[i].strongNeighbors[nextAvailablePos] = sccID;
					    //hyperGraphArray[i].randIndex[nextAvailablePos] = 0;
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
					    	printf("strongNeighbors is full (realloc)\n");
					    	hyperGraphArray[i].size = 2*hyperGraphArray[i].size;

					    	newStrongNeighbors = realloc(hyperGraphArray[i].strongNeighbors, hyperGraphArray[i].size * sizeof(int));
					    	hyperGraphArray[i].strongNeighbors = newStrongNeighbors;

					    	//newRandIndex = realloc(hyperGraphArray[i].randIndex, hyperGraphArray[i].size * sizeof(int));
					    	//hyperGraphArray[i].randIndex = newRandIndex;
					    }
					    hyperGraphArray[i].strongNeighbors[nextAvailablePos] = sccID;
					    //hyperGraphArray[i].randIndex[nextAvailablePos] = 0;
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


void destroyHyperGraph(hyperGraphStruct* hyperGraph, int components_count)
{
	int i;

	for(i=0; i<components_count; i++)
	{
		free(hyperGraph[i].strongNeighbors);
		free(hyperGraph[i].strongNeighborsIndex);
		//free(hyperGraph[i].randIndex);
	}
	free(hyperGraph);
	//printf("\nhyperGraph has been destroyed.\n");
}


/**************************************************************************************************/
/********************************************* GRAIL *********************************************/
void pushFrontier(int sccID, int parent, grailFront *frontier)
{
	int last, size;

	last = frontier -> last;
	size = frontier -> size;
	if(last == size-1)
	{
		printf("^(Frontier is full, realloc)\n");
		frontier -> size = size * 2;
		frontier -> frontArray = realloc(frontier -> frontArray, frontier->size * sizeof(frontierEntry));
	}
	frontier -> frontArray[last+1].sccID = sccID;
	frontier -> frontArray[last+1].parent = parent;
	frontier -> last = last + 1;
}

frontierEntry popFrontier(grailFront *frontier)
{
	int last;

	last = frontier -> last;
	if(last != -1)
	{
		frontier -> last = last- 1;
		return frontier -> frontArray[last];
	}
	else
	{
		printf("Frontier is empty!\n");
		//return NULL;
	}
}

/*
void pushStack(int sccID, grailStack *stack)
{
	int last, size;
	last = stack -> last;
	size = stack -> size;
	
	if(last == size-1)
	{
		printf("Stack is full (realloc)\n");
		stack -> size = size * 2;
		stack -> stackArray = realloc(stack -> stackArray, stack->size * sizeof(int));
	}
	stack -> stackArray[last+1] = sccID;
	stack -> last = last + 1;
	stack -> current = stack -> last;

}*/

int minRankOfChildren(int sccID, hyperGraphStruct *hyperGraph, grailStruct *grailArray)	//efarmozetai se components pou exoun paidia
{
	int i, min, strongNeighbor, nextAvailablePos;

	strongNeighbor = hyperGraph[sccID].strongNeighbors[0];
	min = grailArray[strongNeighbor].minRank;

	nextAvailablePos = hyperGraph[sccID].nextAvailablePos;
	for(i=1; i<nextAvailablePos; i++)
	{
		strongNeighbor = hyperGraph[sccID].strongNeighbors[i];
		if(grailArray[strongNeighbor].minRank < min)
			min = grailArray[strongNeighbor].minRank;
	}
	//printf("scc: %d gets minRank: %d\n", sccID, min); //legit print.
	return min;
}

int grailExpand(int sccID, hyperGraphStruct *hyperGraph, grailFront *frontier, grailStruct *grailArray)
{
	int i, strongNeighbor, unvisitedChildren, randNeighborPos, nextAvailablePos;

	//printf("\n");
	nextAvailablePos = hyperGraph[sccID].nextAvailablePos;
	if(nextAvailablePos == 0)		//simainei oti den exei paidia/geitones
	{
		//printf("scc: %d does not have any neighbors\n", sccID);
		unvisitedChildren = -1;
		return unvisitedChildren;
	}

	unvisitedChildren = 0;
	i = 0;
	while(i < nextAvailablePos)
	{
		randNeighborPos = rand() % nextAvailablePos;
		//printf("randNeighborPos: %d -> strongNeighbors[%d] = ", randNeighborPos, randNeighborPos);
		if(grailArray[sccID].randIndex[randNeighborPos] == 0)
		{
			
			grailArray[sccID].randIndex[randNeighborPos] = 1;
			strongNeighbor = hyperGraph[sccID].strongNeighbors[randNeighborPos];
			//printf("%d accepted.\n", strongNeighbor);
			if( grailArray[strongNeighbor].minRank == 0 && grailArray[strongNeighbor].rank == 0)	//einai unvisited
			{
				printf("^strongNeighbor: %d pushed to the frontier.\n", strongNeighbor);
				unvisitedChildren++;
				grailArray[strongNeighbor].parent = sccID;
				pushFrontier(strongNeighbor, sccID, frontier);	//to sccID edo dilonei parent tou strongNeighbor
				
			}
			i++;
		}
		//else
			//printf("denied(already picked).\n");
	}
	grailArray[sccID].unvisitedChildren = unvisitedChildren;
	return unvisitedChildren;
}

grailIndex* buildGrailIndex(NodeIndex *index, Buffer *buffer, SCC *sccPtr, hyperGraphStruct *hyperGraph)
{
	int unvisitedComponent, randomComp, sccID, expandResult, i, j, gsize;

	grailIndex *grail;
	grailFront *frontier;
	grailStack *stack;
	//hyperGraphStruct *hyperGraph;
	frontierEntry entry;
	grailStruct *grailArray;

	//grail initialization
	grail = malloc(sizeof(grailIndex));
	grail -> index = 1;
	grail -> size = sccPtr -> components_count;

	//frontier initialization
	grail -> frontier = malloc(sizeof(grailFront));
	frontier = grail -> frontier;
	frontier -> frontArray = malloc(1*sizeof(frontierEntry));
	frontier -> size = 1;
	frontier -> last = -1;
	printf("Frontier creation and initialization completed.\n");

	gsize = grail -> size;
	grail -> grailArray = malloc(gsize * sizeof(grailStruct));
	grailArray = grail -> grailArray;


	for(i=0; i<gsize; i++)
	{
		grailArray[i].minRank = 0;
		grailArray[i].rank = 0;
		grailArray[i].parent = -1;
		grailArray[i].unvisitedChildren = -1;
		grailArray[i].randIndexSize = hyperGraph[i].nextAvailablePos;
		grailArray[i].randIndex = malloc(grailArray[i].randIndexSize * sizeof(int));
		for(j=0; j<grailArray[i].randIndexSize; j++)
		{
			grailArray[i].randIndex[j] = 0;
		}
	}

	//stack initialization
	/*
	stack = malloc(sizeof(grailStack));
	stack -> size = 10;
	stack -> current = -1;
	stack -> last = -1;
	printf("Stack creation and initialization completed.\n");
	*/

	unvisitedComponent = gsize;		//visit/eksereunimeno = na parei timi to (minRank, rank)
	while(unvisitedComponent != 0)
	{
		printf("\nUnvisited Components: %d\n", unvisitedComponent);
		//dialekse tixaia ena component
		randomComp = rand() % gsize;
		while(grailArray[randomComp].minRank != 0 || grailArray[randomComp].rank != 0)
			randomComp = rand() % gsize;
		//randomComp = 2; //test only
		printf("randomComp: %d\n", randomComp);
		pushFrontier(randomComp, -1, frontier); //-1 h hyperGraph[randomComp].parent pou einai pali -1 stin arxi.
		while(frontier -> last != -1)	//oso to frontier DEN einai adeio
		{
			entry = popFrontier(frontier);
			sccID = entry.sccID;
			printf("*popped sccID: %d\n", sccID);
			if(grailArray[sccID].minRank != 0 || grailArray[sccID].rank != 0) //na testaristei to paradeigma sto xarti. thelei auto + elegxo stin expand an exei idi timi o pateras na tou meiosei to unvisited. prosoxi 358 break.
			{
				printf("^already visited by parent: %d\n", grailArray[sccID].parent);
				sccID = entry.parent;
				grailArray[sccID].unvisitedChildren--;
			}
			else
			{
				expandResult = grailExpand(sccID, hyperGraph, frontier, grailArray);
				printf("^expandResult: %d\n", expandResult);
				if(expandResult == -1)	//an den exei katholou paidia
				{
					grailArray[sccID].rank = grail -> index;
					grailArray[sccID].minRank = grail -> index;
					unvisitedComponent--;
					grail -> index = (grail -> index) + 1;
					printf("sccID: %d (minRank, rank) = (%d, %d)\n", sccID, grailArray[sccID].minRank, grailArray[sccID].rank);
					//pare ton patera, an den einai o pateras o idios, kai meiose ta unvisitedChildren tou kata 1
					if(sccID != randomComp)
					{
						sccID = grailArray[sccID].parent;
						grailArray[sccID].unvisitedChildren--;
					}
				}
			}

			if(grailArray[sccID].unvisitedChildren == 0)	//an ta paidia einai ola eksereunimena/visited/exoun timi
			{
				while(/*sccID != randomComp &&*/ grailArray[sccID].unvisitedChildren == 0)
				{
					grailArray[sccID].minRank = minRankOfChildren(sccID, hyperGraph, grailArray);
					grailArray[sccID].rank = grail -> index;
					grail -> index = (grail -> index) + 1;
					printf("sccID: %d (minRank, rank) = (%d, %d)\n", sccID, grailArray[sccID].minRank, grailArray[sccID].rank);
					unvisitedComponent--;
					//pare ton patera, an den einai o pateras o idios, kai meiose ta unvisitedChildren tou kata 1
					if(sccID != randomComp)
					{
						sccID = grailArray[sccID].parent;
						grailArray[sccID].unvisitedChildren--;
					}
					else
						break;
				}
			}
		}
		//hyperGraph[randomComp].minRank = minRankOfChildren(randomComp, hyperGraph);
		//hyperGraph[randomComp].rank = grail -> index;
		//grail -> index = (grail -> index) + 1;
	}
	printf("###################################################\n");
	return grail;
}

int isReachableGrailIndex(grailIndex **grailStorage, SCC *sccPtr, int start, int destination)
{
	int sccStart, sccDestination, i;
	grailStruct *grailArray;

	sccStart = sccPtr -> id_belongs_to_component[start];
	sccDestination = sccPtr -> id_belongs_to_component[destination];

	if( sccStart == sccDestination )
	{
		printf("(YES) -> There is a path between startNode: %d(%d) and destinationNode: %d(%d).\n", start, sccStart, destination, sccDestination);
		return 2; //"YES"
	}
	else
	{
		i = 0;
		while(i < numOfGrails)
		{
			grailArray = grailStorage[i] -> grailArray;
			if( (grailArray[sccDestination].minRank >= grailArray[sccStart].minRank) && (grailArray[sccDestination].rank <= grailArray[sccStart].rank) )
			{
				printf("grail: %d gave us MAYBE. go to the next one.\n", i);
				i++;
			}
			else
			{
				printf("(NO) -> There is no path between startNode: %d(%d) and destinationNode: %d(%d).\n", start, sccStart, destination, sccDestination);
				return 0;
			}
		}
		//ama vgei simainei oti ola edosan MAYBE opote...
		printf("(MAYBE) -> There might be a path between startNode: %d(%d) and destinationNode: %d(%d).\n", start, sccStart, destination, sccDestination);
		return 1; //"MAYBE"
	}
	/*
		if( (hyperGraph[sccDestination].minRank >= hyperGraph[sccStart].minRank) && (hyperGraph[sccDestination].rank <= hyperGraph[sccStart].rank) )
		{

			printf("(MAYBE) -> There might be a path between startNode: %d(%d) and destinationNode: %d(%d).\n", start, sccStart, destination, sccDestination);
			return 1; //"MAYBE"
		}
		else
		{
			printf("(NO) -> There is no path between startNode: %d(%d) and destinationNode: %d(%d).\n", start, sccStart, destination, sccDestination);
			return 0; //"NO"
		}*/
}

void destroyGrailIndex(grailIndex **grailStorage)
{
	int i;
	
	for(i=0; i<numOfGrails; i++)
	{
		//destroyHyperGraph(grailStorage[i] -> hyperGraph, grailStorage[i] -> size);
		free(grailStorage[i] -> grailArray);
		free(grailStorage[i] -> frontier -> frontArray);
		free(grailStorage[i] -> frontier);
	}
	//printf("\nEvery structure inside grail has been destroyed.\n");
}