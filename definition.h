#ifndef DEFINITION__H__
#define DEFINITION__H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define STRLEN 1024
#define N 10
#define HASH 5
#define HASHLEN 200

/************* HASH STRUCTS ****************/


typedef struct hash_nodes
{
	int next_available;
	int* hash_array;
	int size;

}hash_nodes;

typedef struct hash_table
{
	hash_nodes* hash_table;

}hash_table;


/******************************************/

typedef struct indexStruct
{
	int offset;
	int tail;
	int neighbor_num;
	int cc;
	int visited;
	int bfs[2];

	hash_table* table;
	
}indexStruct;

typedef struct NodeIndex
{
	indexStruct* index_array;
	int size;
}NodeIndex;

typedef struct list_node
{
        uint32_t neighbor[N];       //the ids of the neighbor nodes
        uint32_t edgeProperty[N];   //property for each edge
        int  nextListNode;
        int emptyNeighborPos;
}list_node;

typedef struct Buffer
{
	list_node* buffer_array;
	int next_available_ba;
	int size;
}Buffer;


/*******************FUNCTIONS***********************/



NodeIndex* createNodeIndex();

void IndexArrayInit(NodeIndex*, int);

Buffer* createBuffer();

void BufferArrayInit(Buffer*, int);

void ListNodeInit(list_node*);

/****************************************************/

void print_index_array(NodeIndex*, int );

void print_buffer(Buffer*, int);

int insertNode(NodeIndex*, Buffer*, uint32_t , uint32_t );

int insertEdge(Buffer* , NodeIndex*, int , uint32_t , uint32_t, int* );

/*****************DELETING STRUCTURE*****************/

int delete_structure(NodeIndex*,NodeIndex* , Buffer*, Buffer*);

int destroyBuffer(Buffer*);

int destroyNodeIndex1(NodeIndex*);

int destroyNodeIndex2(NodeIndex*);

/****************************************************/

int getListHead(NodeIndex*, int );

/**************************************************/

int insertEdgeHash(Buffer* , int , uint32_t , uint32_t , int* );

/*****************UNIT TESTING*****************/

int testInput();

int testMemory(Buffer* , Buffer*, NodeIndex*,  NodeIndex*);

void edges_num_set(int ,int ,NodeIndex* );

/****************HASH FUNCTIONS********************/

int hash_fun(int , int);

hash_table* hash_init(hash_table*);

int hash_add_file(int , int , hash_table**);

int hash_add_query(int , int , hash_table**);

void delete_hash(hash_table*);



/**************************************************/


#endif

