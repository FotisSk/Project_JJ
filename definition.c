#include "definition.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define STRLEN 1024
#define N 10

NodeIndex* createNodeIndex()
{
        NodeIndex* NodeHead=malloc(sizeof(NodeIndex));
        return NodeHead;
}

Buffer* createBuffer()
{
        Buffer* BufferHead=malloc(sizeof(Buffer));
        BufferHead->next_available_ba=0;
        return BufferHead;
}

void IndexArrayInit(NodeIndex* ptr, int size)
{
	if(ptr == NULL)
	{
		printf("Index array_pointer does not exist! \n");
		return;
	}

        indexStruct* index_array;
        int i;

        index_array = malloc(size*sizeof(indexStruct));
        for(i=0 ; i < size ; i++)
        {
           	index_array[i].offset = -1;
           	index_array[i].tail = -1;
			index_array[i].visited = -1;
			index_array[i].tarjanvisited = -1;
			index_array[i].cc = -1;
			index_array[i].lowlink = -1;
			index_array[i].index = -1;
			index_array[i].inFrontier = 0;
			index_array[i].inStack = 0;
        }
        ptr->index_array = index_array;
		ptr->size=size;
}

void BufferArrayInit(Buffer* ptr,int size)
{

	if(ptr == NULL)
        {
                printf("Buffer array_pointer does not exist! \n");
                return;
        }

        list_node* buffer_array;
        int i;
        int j;
        buffer_array = malloc(size * sizeof(list_node));
        for(i=0; i < size ; i++)
        {
/*
            for(j=0; j<N ; j++)
            {
			    buffer_array[i].neighbor[j]=-1;
                buffer_array[i].edgeProperty[j]=-1;
            }
*/ 
          buffer_array[i].nextListNode=-1;
          buffer_array[i].emptyNeighborPos = 0;
        }
	ptr->size=size;
    ptr->buffer_array = buffer_array;

}


int insertNode(NodeIndex* index, Buffer* buffer, uint32_t node1 , uint32_t node2)
{


	if( (index == NULL) || (buffer == NULL) )
	{
		printf("Error!! Index array or Buffer array does not exist. \n");
		return -1;
	}

	if( node1 == node2 )
	{
		printf("Circles are not accepted! \n");
		return -1;
	}

	int *SIZE_i=&(index->size);
  	int position, i, originalSize_i;
	indexStruct *index_array, *new_index_array;

	index_array = index->index_array;
	originalSize_i = (*SIZE_i);

//	if( node1 < 0 || node2 < 0 )
//	{
//		printf("Error! Wrong node_num given! \n");
//		return -1;
//	}

	if( node1 >= (*SIZE_i))
	{
		//printf("You have exceeded the Index_Array length! \n");
		while(node1 >= (*SIZE_i))
		{
			(*SIZE_i) = 2*(*SIZE_i);
		}

		new_index_array = realloc(index_array,(*SIZE_i)*sizeof(indexStruct));
		index->index_array = new_index_array;
		index_array = index->index_array;

		for ( i=originalSize_i; i<(*SIZE_i); i++)
		{
			index_array[i].offset = -1;
			index_array[i].tail = -1;
			index_array[i].cc = -1;
		}

		index_array[node1].offset = buffer->next_available_ba;
		index_array[node1].tail = buffer->next_available_ba;
		position = index_array[node1].tail;
		insertEdge(buffer, index, position, node1, node2, &(buffer->size));
	}
	else if ( index_array[node1].offset != -1 )
	{
		//printf("The node given already exists! \n");
        	position = index_array[node1].tail;
        	insertEdge(buffer, index, position, node1, node2, &(buffer->size));

	}
	else
	{
		//printf("The node given does not exist! \n"); //dinoume timi offset kai tail kai mpainoume me tail. to offset den to ksanapeirazoume
		index_array[node1].offset = buffer->next_available_ba;
		index_array[node1].tail = buffer->next_available_ba;
        	position = index_array[node1].tail;
        	insertEdge(buffer, index, position, node1, node2, &(buffer->size));
	}
//	printf("1 \n");
}

int insertEdge(Buffer* buffer , NodeIndex* index, int position, uint32_t node1, uint32_t node2, int* SIZE_b)
{
	int newPosition, i, j, emptyNeighborPos;
	list_node *buffer_array, *new_buffer_array;
	indexStruct *index_array;

	buffer_array = buffer->buffer_array;

	if(position >= (*SIZE_b))
	{
		//printf("You have exceeded the buffer size! %d >= %d \n", position, *SIZE_b); //desmeuoume extra mnimi kai allazoume kai to next_available_ba (tha ginei +1)

		(*SIZE_b) = 2*(*SIZE_b);
		//printf("newSize: %d\n", *SIZE_b);
		new_buffer_array = realloc(buffer_array, (*SIZE_b) * sizeof(list_node));
		if(new_buffer_array == NULL)
		{
			printf("%s\n",strerror(errno));
			//new_buffer_array = realloc(buffer_array, (*SIZE_b) * sizeof(list_node));
		}
		buffer->buffer_array = new_buffer_array;		//ananeosi tou deikti mesa sto struct Buffer
		buffer_array = buffer->buffer_array;			//ananeosi tis metavlitis buffer_array na deixnei sto new

		//printf("Arxi arxikopoihshs\n");
		//arxikopoihsh tou kainourgiou kommatiou
		for(i=position; i < (*SIZE_b) ; i++)
		{
//	    	if(position == 131072)	printf("i: %d\n", i);
	    	//printf("Size_b: %d\n", *SIZE_b);
	    	//printf("You have exceeded the buffer size! 1 \n");
	        /*for(j=0; j<N ; j++)
	        {
	        	//if(position == 131072)	printf("j: %d\n", j);
				buffer_array[i].neighbor[j] = -1;
	            buffer_array[i].edgeProperty[j] = -1;
	        }*/
	        //printf("Exited\n");
	        buffer_array[i].nextListNode=-1;
	        buffer_array[i].emptyNeighborPos = 0;
	    	}
	    //printf("Telos arxikopoihshs\n");
	    newPosition = position;
	    emptyNeighborPos = buffer_array[newPosition].emptyNeighborPos;

		buffer_array[newPosition].neighbor[emptyNeighborPos] = node2;
		buffer_array[newPosition].emptyNeighborPos++;

		//if(buffer_array[newPosition].emptyNeighborPos == 1){ //tote simainei oti molis grapsame kati gia proti fora edo
			buffer -> next_available_ba++;
		//}
	}
	else
	{

		emptyNeighborPos = buffer_array[position].emptyNeighborPos;
		if(emptyNeighborPos >= 0 && emptyNeighborPos < N)
		{
			emptyNeighborPos = buffer_array[position].emptyNeighborPos;
			buffer_array[position].neighbor[emptyNeighborPos] = node2;
			//printf("Edge | %d -> %d | added successfully.\n", node1 , node2);
			//printf("\n");

			buffer_array[position].emptyNeighborPos++;

			if(buffer_array[position].emptyNeighborPos == 1){ //tote simainei oti molis grapsame kati gia proti fora edo
				buffer -> next_available_ba++;
				//printf("nextAvailable_ba: %d\n\n", buffer -> next_available_ba); 
			}
		}
		else
		{
			//printf("Node | %d | neighbor list has exceeded.\n",node1);
			newPosition = buffer -> next_available_ba;

			if(newPosition >= (*SIZE_b))
			{
				//printf("You have exceeded the buffer size! 2 \n"); //desmeuoume extra mnimi kai allazoume kai to next_available_ba (tha ginei +1)

				(*SIZE_b) = 2*(*SIZE_b);
				new_buffer_array = realloc(buffer_array, (*SIZE_b)*sizeof(list_node));
				buffer->buffer_array = new_buffer_array;		//ananeosi tou deikti mesa sto struct Buffer
				buffer_array = buffer->buffer_array;			//ananeosi tis metavlitis buffer_array na deixnei sto new

				//arxikopoihsh tou kainourgiou kommatiou
				for(i=newPosition; i < (*SIZE_b) ; i++)
		        {
		            /*for(j=0; j<N ; j++)
		            {
					    buffer_array[i].neighbor[j]=-1;
		                buffer_array[i].edgeProperty[j]=-1;
		            }*/
		            buffer_array[i].nextListNode=-1;
		            buffer_array[i].emptyNeighborPos = 0;
		        }
			}
			emptyNeighborPos = buffer_array[newPosition].emptyNeighborPos;

			buffer_array[position].nextListNode = newPosition;
			buffer_array[newPosition].neighbor[emptyNeighborPos] = node2;
			buffer_array[newPosition].emptyNeighborPos++;

			index_array = index -> index_array;
			index_array[node1].tail = newPosition;


			//if(buffer_array[newPosition].emptyNeighborPos == 1){ //tote simainei oti molis grapsame kati gia proti fora edo
				buffer -> next_available_ba++;
			//}
		}
	}
}

/************************************************************************/
/****************************** PRINTS *********************************/

void print_index_array(NodeIndex* index, int SIZE_i)
{

	if( index ==  NULL)
                return;

	indexStruct* index_array=index->index_array;
	int i;

	printf("->PRINTING INDEX ARRAY.. \n");
	printf("|==========================================| \n");

	for(i=0 ; i<SIZE_i ; i++)
	{
		printf("Node | %d | - Offset | %d | - Tail | %d | -  CC  | %d |\n",i,index_array[i].offset, index_array[i].tail, index_array[i].cc);
	}
	printf("|==========================================| \n");



}

void print_buffer(Buffer* buffer, int SIZE_b)
{

	if( buffer ==  NULL)
		return;

	int i,j;
	int *index_array;
	list_node *buffer_array;

	buffer_array = buffer->buffer_array;

	printf("->PRINTING BUFFER ARRAY.. \n");
    printf("|==========================================|\n");
	for(i=0; i<SIZE_b; i++)
	{
		printf("Position | %d | *** Neighbor[] = | ",i );
		for(j=0; j<N; j++)
		{
			printf("%d | ", buffer_array[i].neighbor[j] );
		}
		printf("Next list node: | %d |\n", buffer_array[i].nextListNode);
		printf("Empty Neighbor Position: | %d |\n", buffer_array[i].emptyNeighborPos);

	}

	return;

}

/************************************ DELETE THE WHOLE STRUCTURE ***************************************/
/******************************************************************************************************/

int delete_structure( NodeIndex* index, Buffer* buffer )
{

//	printf("|===============================================================|\n");
//	printf("Structure deleting procedure.. \n");

	if( (index==NULL) || (buffer==NULL) )
	{
		printf("You can't delete NULL_valued pointers!! \n");
		return -1;
	}
	int size_index=index->size;
	int size_buffer=buffer->size;

	int i=0;
	int j=0;

	destroyBuffer(buffer);
	destroyNodeIndex(index);

//	printf("The structure has been successfully deleted! \n");
//	printf("|===============================================================|\n");

	return 1;

}

int destroyBuffer( Buffer* buffer )
{
	if(buffer==NULL)
	{
//		printf("Buffer_ptr already NULL!! \n");
		return -1;
	}
	free(buffer->buffer_array);
	free(buffer);
	buffer=NULL;
//	printf("The BUFFER_ARRAY has been successfully deleted! \n");
	return 1;

}


int destroyNodeIndex( NodeIndex* index )
{
	if(index==NULL)
	{
//		printf("Index_ptr already NULL!! \n");
		return -1;
	}
	free(index->index_array);
    free(index);
    index=NULL;
	printf("The INDEX_ARRAY has been successfully deleted! \n");
	return 1;

}

/****************************************************************************************************/
/******************************************GET_LIST_HEAD********************************************/


int getListHead(NodeIndex* index, int nodeId)
{
	if( ( nodeId > (index->size-1) ) || ( nodeId < 0 ) )
	{
		printf("The node asked does not exist!! \n");
		return -1;
	}

	return index->index_array[nodeId].offset;
}


/****************************************************************************************************/

int hash_fun(int node, int length)
{
	int i;
	i= node % length;
	return i;
}


