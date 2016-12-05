#ifndef DEFINITION__H__
#define DEFINITION__H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define STRLEN 1024
#define N 15
#define HASH 200

typedef struct hash_node
{
	int* hash_table[HASH];
}hash_node;

typedef struct indexStruct
{
	int offset;
	int tail;
	int cc;
	int visited;//episkepsi//////////
	int tarjanvisited;
	
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

int delete_structure(NodeIndex*, Buffer*);

int destroyBuffer(Buffer*);

int destroyNodeIndex(NodeIndex*);

/****************************************************/

int getListHead(NodeIndex*, int );

/**************************************************/

int hash_fun(int , int);

int insertEdgeHash(Buffer* , int , uint32_t , uint32_t , int* );

/*****************UNIT TESTING*****************/

int testInput();

int testMemory(Buffer* , Buffer*, NodeIndex*,  NodeIndex*);

#endif

