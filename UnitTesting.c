#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "Bfs.h"
#define STRLEN 1024

int testInput()
{
	FILE *fp;
	int i,j, node1, node2;
	char* str_token;
	char* str=malloc(STRLEN*sizeof(char));
	memset(str,0,STRLEN);

	printf("\n*******************************\n");
	printf("***** UnitTesting (INPUT) *****\n");

	fp=fopen("testInput1.txt", "r");
	if(fp==NULL)
	{
		perror ( " fifo open error: " );
             exit (1);
	}
	j = 1;

	char* original_buf=malloc(STRLEN);
    memset(original_buf,0,STRLEN);
	while(1)
    {
	    	fgets(str,STRLEN,fp);

	 		i = 0;
	    	if(str[0]=='\n')
	    	{
	    		printf("%d. Empty line found.\n", j);
	    		j++;
	            continue;
	    	}

	        str[strlen(str)-1] = '\0';

			if( ( !strcmp(str,"S")) || ( !strcmp(str,"s") ) )
	        {
	            printf("%d. Termination character 'S' found.\n", j);
	            return 1;
	        }

	        strcpy(original_buf,str);

	        str_token=strtok(original_buf,"\t");

	        while(str_token != NULL)
	        {
				i++;
	            str_token=strtok(NULL,"\t");
	        }

	        if( i!=2 )
	        {
				printf("%d. Not acceptable: [command is wrong]\n", j);
	            j++;
	            continue;

	        }

	        str_token=strtok(str,"\t");
			node1=atoi(str_token);
			if(node1 < 0)
			{
				printf("%d. Not acceptable: [Node1 is negative]\n", j);
				j++;
				continue;
			}

			str_token=strtok(NULL,"\t");
			node2=atoi(str_token);
			if(node2 < 0)
			{
				printf("%d. Not acceptable: [Node2 is negative]\n", j);
				j++;
				continue;
			}
			if (node1 == node2)
			{
				printf("%d. Not acceptable: [Node1 = Node2]\n", j);
				j++;
				continue;
			}

			printf("%d. Edge %d -> %d is acceptable.\n",j, node1,node2);
			j++;

			memset(str,0,STRLEN);
			memset(original_buf,0,STRLEN);
	}
	free(original_buf);
	free(str);
	fclose(fp);
}

/*int testMemory(Buffer* buffer1, Buffer* buffer2, NodeIndex* index1, NodeIndex* index2)//gia length = 4 kai N = 4 //de xreiazomaste elegxous afou emeis vazoume tous komvous. na vgoun
{
	FILE *fp;
	int node1, node2, i;
	int *index_array;
	list_node *buffer_array;
	Path* path;
	Frontier* frontier;
	Path* path2;
	Frontier* frontier2;

	printf("\n********************************\n");
	printf("***** UnitTesting (MEMORY) *****\n");

	char* str_token;
	char* str=malloc(STRLEN*sizeof(char));
	memset(str,0,STRLEN);

	fp=fopen("testInput3.txt", "r");
		if(fp==NULL)
		{
			perror ( " fifo open error: " );
                        exit (1);
		}

	while(1)
	{
		fgets(str,STRLEN,fp);
		if(str[0]=='\n')
            continue;

        str[strlen(str)-1] = '\0';

		//printf("STRING ADD: |%s|",str);

        if( (!strcmp(str,"S")) || (!strcmp(str,"s")) )
        {
            printf("Graph created!\n");
			break;
        }

        str_token=strtok(str,"\t");
        node1=atoi(str_token);

        str_token=strtok(NULL,"\t");
        node2=atoi(str_token);

        printf("The edge given is %d -> %d\n",node1,node2);

		insertNode( index1 ,buffer1, node1, node2 );
		insertNode( index2 , buffer2, node2, node1);
    }
    printf("Size of Index Array1: %d\n", index1->size);
    index_array=index1->index_array;
    printf("Size of Buffer Array1: %d\n", buffer1->size);
    buffer_array=buffer1->buffer_array;
    printf("Print all the neighbors of node 2\n[   ");

    int offset = index_array[2];
    i = 0;

    while(buffer_array[offset].nextListNode != -1)
    {
    	for(i=0; i<N; i++)
    	{
    		printf("%d   ", buffer_array[offset].neighbor[i]);
    	}
    	//if(i == N)
    	//{
    		offset = buffer_array[offset].nextListNode;
    	//}	
    }
    int j = 0;
    while(buffer_array[offset].neighbor[j] != -1)
    {
    	printf("%d   ", buffer_array[offset].neighbor[j] );
    	j++;
    }
    printf("]\n");
    /********************************************************************/
    /*******************************BFS*********************************/
    /*printf("\n**********************************\n");
	printf("******** UnitTesting (BFS) *******\n");

    int flag = 0;
    path = path_init();
	frontier = frontier_init();
	path2 = path_init();
	frontier2 = frontier_init();
	printf("Find path from 5 to 11.\n");
	printf("Expected path: 5 -> 3 -> 11 with cost 2.\n");
	push_frontier(5, 5, frontier);
	push_frontier(11, 11, frontier2);
	printf("Search started\n");
	while (flag != 1)
	{
		flag = bidirectional(frontier, path, 11, index1, buffer1, frontier2, path2, 5, index2, buffer2);
		if (flag == 0) {
			printf("-1");
			break;
		}
	}
	if (flag == 1) {
		print_path(path, path2);
	}
	flag = 0;
/******************************DELETES***************************/
	/*free(path->path_array);
	free(path);
	free(frontier->nodes_to_expand);
	free(frontier);

	free(path2->path_array);
	free(path2);
	free(frontier2->nodes_to_expand);
	free(frontier2);

    /******************************DELETES***************************/
    /*fclose(fp);
    free(str);
    delete_structure(index1,buffer1);
    delete_structure(index2,buffer2);
    return 1;

	
 		
}*/