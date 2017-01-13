#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "Bfs.h"
#include <time.h>
#include "SCC.h"
#include "grail.h"

#define STRLEN 1024
#define ARRAYLEN 2000
#define numOfGrails 3
#define N 10

int main(int argc,char* argv[])
{
	//clock_t begin=clock();

	if( argc != 2 )
	{
		printf("Wrong command line arguments given!! \n");
		printf("The program will end! \n");
		exit(1);
	}

/***************** BUFFER AND INDEX INITIALIZATION******************************/

	int array_length=atoi(argv[1]);
//1
	NodeIndex* index=createNodeIndex();
	IndexArrayInit(index, array_length);
//2
	NodeIndex* index2=createNodeIndex();
	IndexArrayInit(index2, array_length);

//1
	Buffer* buffer=createBuffer();
	BufferArrayInit(buffer, array_length);
//2
	Buffer* buffer2=createBuffer();
    	BufferArrayInit(buffer2, array_length);

//////////////////////////////////////////////////////////////////////////////////

	Frontier* frontier;
	Frontier* frontier2;
	int cycle = 0;//kiklos tis bfs
	int flag = 0;

	FILE *fp=NULL;

	int node1,node2;
	char* mode=malloc(STRLEN);
	memset(mode,0,STRLEN);
	char ch;

/***************************************PHASE 1***************************************/

/*	printf("Give | F | to read from file or | U | to start Unit Testing mode \n");			//UNCOMMENT TO REACH UNIT+_+TESTING
	while(1)
	{
		fgets(mode,STRLEN,stdin);
		if( mode[0]=='\n' )
			continue;
		mode[strlen(mode)-1]='\0';
		if( (!strcmp(mode, "F")) || (!strcmp(mode, "U")) )
		{
			break;
		}
		printf("Wrong option given! Try again.. \n");
	}

	if(!strcmp(mode, "F"))
	{
*/
		fp=fopen("mediumGraph.txt", "r");

		if(fp==NULL)
		{
			perror ( " fifo open error: " );
        	        exit (1);
		}
		else
		{
			int neighbor_num=0;
			int node_temp=-1;
			int* edge_array=malloc(ARRAYLEN*sizeof(int));
			int size_temp=ARRAYLEN;
			int ct;
			for(ct = 0 ; ct < ARRAYLEN; ct++)
			{
				edge_array[ct] = -1;
			}

			int prev_len=0;
			int hash_num;


			while(1)
			{


				if (fscanf(fp, "%d	%d", &node1, &node2) !=2)
				{

					if(edge_array != NULL)
					{
			//			printf("1.1 \n");
						free(edge_array);
						edge_array=NULL;
			//			printf("1.2 \n");
					}
/***************************ELEGXOS UPARKSHS AKMHS KAI EISAGWGH**********************************************/

					printf("First phase has ended! \n");
					break;
				}

/**************************************************************************************************************/
	//			printf("As mpw ligo sta limeria \n");
				if( node1 != node_temp)							//Otan allaksei o komvos pou pername tis akmes
				{

					if(node_temp != -1)
					{
					//	printf("Allazw komvo eisagwghs \n");
						edges_num_set(node_temp , neighbor_num, index);
					}

					node_temp=node1;
					neighbor_num=0;
				}
				if(node2 < size_temp)						//An xwraei o komvos ston pinaka
				{

					if(edge_array[node2] == node1)
					{
						printf("Edge | %d | -> | %d | already exists!!. \n",node1,node2);
						continue;
					}										
					edge_array[node2]=node1;
					neighbor_num++;
				}
				else 										
				{
					prev_len=size_temp;
					while(node2 >= size_temp)
					{	
						(size_temp) = 2*(size_temp);
					}

			//		printf("1 \n");
		//			printf("SIZE IS %d \n",size_temp);
					edge_array=realloc(edge_array,size_temp*sizeof(int));
					
					for(ct = prev_len ; ct < size_temp ; ct++)
					{
						edge_array[ct] = -1;
					}
					edge_array[node2]=node1;
					neighbor_num++;
	//				printf("2 \n");
				}
				

/****************************TELOS EURESHS UPARXOUSAS AKMHS ****************************************/
		//		printf("Akrivws prin tis add \n");
				insertNode( index ,buffer, node1, node2);
				insertNode( index2 , buffer2, node2, node1);

			//	printf("Kapaki meta tis add \n");

				if( neighbor_num == 1 )
				{
				//	printf("Initiaize Node1: | %d | \n",node1);
					index->index_array[node1].table = hash_init( index->index_array[node1].table );

				}
				hash_num=hash_fun(HASH , node2);
				hash_add_file( hash_num , node2 , &( index->index_array[node1].table ) );
				

/***************************** HASH INSERT *********************************************************/


/***************************************************************************************************/

			}
			free(edge_array);
//		}											//T+_+T

/******************************************************************************************************/
	}

	fclose(fp);

	int cc=0;
	printf("1 \n");
	create_cc(index, index2, buffer, buffer2,&cc);
	printf("Final CC is | %d | \n", cc) ;
	//print_index_array(index, index -> size);
	//print_buffer(buffer, buffer->size);

        /*************************************** ADD EDGES AND QUERIES ************************************************/
//	printf("2 \n");
	SCC* scc=SCC_create();
	printf("Teleiwsa to SCC_create \n");

	SCC_initialize(scc, 20 , index);//index->size

	printf("Prin ton tarjan \n");
	tarjan(index, index2, buffer, buffer2, scc);
	/*
	for (int i = 0; i < scc->components_count; i++) {
		scc->components[i].component_id;
		printf("Component No: %d \n", i);
		for (int y=0; y < scc->components[i].included_nodes_count; y++) {
			printf(" Node %d \n", scc->components[i].included_node_ids[y]);
		}
	}
	*/
	printf("Bghka apo ton Tarjan \n");

	//grail arxi
	
	//clock_t begin=clock();

	printf("Starting hyperGraph creation.\n\n");
	hyperGraphStruct *hyperGraph = createHyperGraph(buffer, index, scc);
	printf("Finished hyperGraph creation.\n");

	///clock_t end = clock();
	//double time_spent=(double)(end-begin);
    //printf("Program Duration: %f \n",(time_spent/CLOCKS_PER_SEC) );
	//printf("Starting grail creation.\n\n");

	int i;
	grailIndex *grail, **grailStorage;

	clock_t begin=clock();
	
	srand(time(NULL));
	grailStorage = malloc(numOfGrails * sizeof(grailIndex));
	for(i=0; i<numOfGrails; i++)
	{
		grail = buildGrailIndex(index, buffer, scc, hyperGraph);
		grailStorage[i] = grail;
	}
	printf("\nGrail construction has been completed.\n");

	clock_t end = clock();
	double time_spent=(double)(end-begin);
    printf("Program Duration: %f \n",(time_spent/CLOCKS_PER_SEC) );
	printf("Starting grail creation.\n\n");


	isReachableGrailIndex(grailStorage, scc, 6, 0);
	isReachableGrailIndex(grailStorage, scc, 3, 6);
	isReachableGrailIndex(grailStorage, scc, 4, 1);
	isReachableGrailIndex(grailStorage, scc, 5, 9);
	isReachableGrailIndex(grailStorage, scc, 9, 15);
	isReachableGrailIndex(grailStorage, scc, 9, 14);
	isReachableGrailIndex(grailStorage, scc, 13, 14);
	isReachableGrailIndex(grailStorage, scc, 12, 10);
	isReachableGrailIndex(grailStorage, scc, 17, 7);
	isReachableGrailIndex(grailStorage, scc, 12, 1);
	isReachableGrailIndex(grailStorage, scc, 14, 7);

	destroyHyperGraph(hyperGraph, scc->components_count);
	destroyGrailIndex(grailStorage);
	free(grailStorage);
	printf("\nGrail has been destroyed\n");

	//clock_t end = clock();
	//double time_spent=(double)(end-begin);
    //printf("Program Duration: %f \n",(time_spent/CLOCKS_PER_SEC) );

	//grail telos
	return 0;

	char str;

    printf("|==================================================| \n");
	printf("You have entered the next Phase of << Edge addition >> and << Queries >> \n");

	fp=fopen("workload1.txt", "r");

	if(fp==NULL)
    {
        perror ( " fifo open error: " );
        exit (1);
    }
	int selection;
	
/* DHMIOURGW TO PINAKAKI HELLO */
	QueryComp* ptr;
	ptr=edge_table(ptr);

	while(1)
	{
				selection=fscanf(fp,"%c       %d      %d\n",&ch,&node1, &node2);

				if ( ch=='F')
				{
					continue;
					clock_t end=clock();
                    double time_spent=(double)(end-begin);
                    printf("Program Duration: %f \n",(time_spent/CLOCKS_PER_SEC) );
                    continue;
				}
				else if (ch=='B')
				{
					clock_t end=clock();
                    double time_spent=(double)(end-begin);
                    printf("Program Duration: %f \n",(time_spent/CLOCKS_PER_SEC) );

				/*********************DESTROYING INDEX1 && BUFFER1************************************************/

                    delete_structure( index, index2, buffer, buffer2 );


                /*************************************************************************************************/

					return 1;
				}
				else if ( ch=='A')
				{
					if( ( node1 < 0 ) || ( node2 < 0 ) )
					{
						printf("Negative nodes not accepted.\n");
						printf("|==================================================| \n");
						continue;
					}
				


/*********************************** HASH CALLS IN MAIN ************************************************************/
					int hash_num=hash_fun(HASH , node2);
			//		printf("Edge given is | %d | -> | %d | \n",node1,node2);(*ptr)->hash_table[hashnum].next_available
			//		printf(" %d \n", index->index_array[node1].table->hash_table[hash_num].next_available);

					if( node1 < ( index->size ) )
					{
						if( index->index_array[node1].table == NULL)
						{
							index->index_array[node1].table = hash_init( index->index_array[node1].table );
						}
						if( hash_add_query (hash_num , node2 , &( index->index_array[node1].table )  ) == 0  )
							continue;
						insertNode( index , buffer, node1, node2);
						insertNode( index2 , buffer2, node2, node1);
					} 
					else
					{
						insertNode( index , buffer, node1, node2);
						insertNode( index2 , buffer2, node2, node1);
						index->index_array[node1].table = hash_init( index->index_array[node1].table);
						hash_add_query(hash_num , node2 , &( index->index_array[node1].table));

					}

/*********************************************************************************************************************/

					//printf("Node1:| %d | Node2: | %d | \n",node1,node2);
				//	weekly(node1, node2, index, index2,  &cc, ptr);

				}
				else if (ch=='Q')
				{
					continue;
				//	printf("Mphka sthn BFS. \n");
					if( ( node1 < 0)||( node2 < 0 ) )
					{
						printf("Negative nodes not accepted.\n");
						printf("|==================================================| \n");
						continue;
					}

					if(  (node1 > (index->size - 1) ) || (node2 > (index2->size-1)) )
					{
						printf("Node1 or Node2 given exceeds the index_array size.. \n");
						continue;
					}
					flag = bidirectional2(node2, index, buffer, node1, index2, buffer2, cycle);
					cycle++;
				}
				else
					printf("Wrong command given! Try again.. \n");

				//printf("|==================================================| \n");

	}
	printf("|==================================================| \n");

}


/*
	else if( !strcmp(str2, "U") )
	{
			free(str2);

			printf("\n##################################################\n");
			printf("###     You have entered UnitTesting mode.     ###\n");
			printf("##################################################\n");

			printf("\n[Input, Memory, and BFS will be tested]\n");
			printf("Press 'B' to begin testing.\n");
			printf("Press 'E' to exit.\n");
			//if(testInput(node1, node2) == 1)
					//printf("** testInput has exited normally **\n\n");
					//return 0;

			ch2 = getchar();
			while(ch2 == '\n')
				ch2 = getchar();

			while(ch2 != 'B' && ch2 != 'E')
			{
				printf("Wrong input. Try again\n");
				ch2 = getchar();
				while(ch2 == '\n')
					ch2 = getchar();
			}
			if(ch2 == 'B')
			{
				//if(testInput() == 1)
					printf("** testInput has exited normally **\n\n");

				//if(testMemory(buffer, buffer2, index, index2) == 1) //xoris elegxous mono na xoristei to string sta nodes kai klisi sinartiseon
					printf("** testMemory has exited normally **\n\n");
			}
			else if(ch2 == 'E')
			{
				printf("UnitTesting mode exited successfully.\n");
				return 0;
			}
	}
	else
	{
		printf("Error. Wrong parameter given!! \n");
		exit(1);
	}
}

*/
