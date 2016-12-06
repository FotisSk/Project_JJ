#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#define EDGELEN 15



int main(void)
{
	int i,j;
	int **table;
	table=malloc(EDGELEN*sizeof(int*));
	for(i=0 ; i < EDGELEN ; i++)
	{
		table[i]=malloc( 2*sizeof(int) );
	}

	for(i=0 ; i < EDGELEN ; i++)
	{
		printf("Table[%d][0] = %d \n",i,table[i][0]);
		printf("Table[%d][1] = %d \n",i,table[i][1]);
	}

}
