#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "Bfs.h"
#include <time.h>
#include "SCC.h"
#include "grail.h"
#define STRLEN 1024
#define N 10


int main(int argc,char* argv[])
{
	clock_t begin=clock();

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
		fp=fopen("grailTest.txt", "r");

		if(fp==NULL)
		{
			perror ( " fifo open error: " );
        	        exit (1);
		}
		else
		{
			while(1)
			{


				if (fscanf(fp, "%d	%d", &node1, &node2) !=2)
				{
					printf("First phase has ended! \n");
					break;
				}
//				printf("Node1: | %d | - Node2: | %d |. \n",node1,node2);
				insertNode( index ,buffer, node1, node2);
				insertNode( index2 , buffer2, node2, node1);
			}
//		}											//T+_+T
	}
	fclose(fp);

	int cc=0;
	create_cc(index, index2, buffer, buffer2,&cc);
	printf("Final CC is | %d | \n", cc) ;
	print_index_array(index, index -> size);
	print_buffer(buffer, buffer->size);

//	print_index_array(index2, index2 -> size);
//	print_buffer(buffer2, buffer2->size);
        /*************************************** ADD EDGES AND QUERIES ************************************************/

	SCC* scc=SCC_create();

	printf("Teleiwsa to SCC_create \n");

	SCC_initialize(scc, index->size , index);

	printf("Teleiwsa to Initialize \n");

	//example gia to grail diko mou. ipotheto oti exei treksei tarjan
	scc -> components_count = 6;

	scc -> components[0].component_id = 0;
	scc -> components[0].included_nodes_count = 3;
	scc -> components[0].included_node_ids[0] = 0;
	scc -> components[0].included_node_ids[1] = 1;
	scc -> components[0].included_node_ids[2] = 2;

	scc -> components[1].component_id = 1;
	scc -> components[1].included_nodes_count = 3;
	scc -> components[1].included_node_ids[0] = 3;
	scc -> components[1].included_node_ids[1] = 4;
	scc -> components[1].included_node_ids[2] = 5;

	scc -> components[2].component_id = 2;
	scc -> components[2].included_nodes_count = 4;
	scc -> components[2].included_node_ids[0] = 6;
	scc -> components[2].included_node_ids[1] = 7;
	scc -> components[2].included_node_ids[2] = 8;
	scc -> components[2].included_node_ids[3] = 9;

	scc -> components[3].component_id = 3;
	scc -> components[3].included_nodes_count = 1;
	scc -> components[3].included_node_ids[0] = 10;

	scc -> components[4].component_id = 4;
	scc -> components[4].included_nodes_count = 1;
	scc -> components[4].included_node_ids[0] = 11;

	scc -> components[5].component_id = 5;
	scc -> components[5].included_nodes_count = 1;
	scc -> components[5].included_node_ids[0] = 12;

	scc -> id_belongs_to_component[0] = 0;
	scc -> id_belongs_to_component[1] = 0;
	scc -> id_belongs_to_component[2] = 0;

	scc -> id_belongs_to_component[3] = 1;
	scc -> id_belongs_to_component[4] = 1;
	scc -> id_belongs_to_component[5] = 1;

	scc -> id_belongs_to_component[6] = 2;
	scc -> id_belongs_to_component[7] = 2;
	scc -> id_belongs_to_component[8] = 2;
	scc -> id_belongs_to_component[9] = 2;

	scc -> id_belongs_to_component[10] = 3;

	scc -> id_belongs_to_component[11] = 4;

	scc -> id_belongs_to_component[12] = 5;
	
	hyperGraphStruct* hyperGraph = createHyperGraph(buffer, index, scc);
	printf("Vgika apocreateHyperGraph.\n");
	return 0;
	//telos example gia to grail.
	printf("PRin ton tarjan \n");
	tarjan(index, index2, buffer, buffer2, scc);
	printf("Bghka apo ton Tarjan \n");


	char str;

    printf("|==================================================| \n");
	printf("You have entered the next Phase of << Edge addition >> and << Queries >> \n");


	fp=fopen("workload.txt", "r");

	if(fp==NULL)
    {
        perror ( " fifo open error: " );
        exit (1);
    }
	int selection;
	
/* DHMIOURGW TO PINAKAKI HELLO */
	QueryComp* ptr;
	ptr=edge_table(ptr);

	//printf("Paw na printarw to table \n");
//	print_edge_table(ptr);
	//printf("Telos me to print tou table chill \n");


	while(1)
	{
				selection=fscanf(fp,"%c       %d      %d\n",&ch,&node1, &node2);

				if ( ch=='F')
				{
					continue;
					clock_t end=clock();
                    double time_spent=(double)(end-begin);
                    printf("Program Duration: %f \n",(time_spent/CLOCKS_PER_SEC) );
				}
				if (ch=='B')
				{
					clock_t end=clock();
                    double time_spent=(double)(end-begin);
                    printf("Program Duration: %f \n",(time_spent/CLOCKS_PER_SEC) );
				//	print_edge_table(ptr);
				//	printf("The CC | %d | \n",cc );
					return 1;
				}

				if (selection != 3)
                {

                    printf("First phase has ended! \n");
                    delete_structure(index,buffer);
                    delete_structure(index2,buffer2);
//					clock_t end=clock();
//                  double time_spent=(double)(end-begin);
//                  printf("Program Duration: %f \n",(time_spent/CLOCKS_PER_SEC) );
                     return 1;
                }

//				printf("Option: |%c| - Node1: |%d| - Node2: |%d|. \n",ch,node1,node2);

				if ( ch=='A')
				{
					if( ( node1 < 0 ) || ( node2 < 0 ) )
					{
						printf("Negative nodes not accepted.\n");
						printf("|==================================================| \n");
						continue;
					}

					insertNode( index , buffer, node1, node2);
					insertNode( index2 , buffer2, node2, node1);
					//printf("Node1:| %d | Node2: | %d | \n",node1,node2);
					weekly(node1, node2, index, index2,  &cc, ptr);

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
