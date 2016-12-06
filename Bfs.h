#ifndef BFS__H__
#define BFS__H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "definition.h"
#define STRLEN 1024
//iuytfyutfcjytgcjykcvjkgtycvf

typedef struct PathNode
{
	int node;
	int parent_node;
}PathNode;

typedef struct Path
{
	int size;
	int count;
	PathNode* path_array;
}Path;

typedef struct VisitedList
{
	int size;
	int count;
	int* path_array;
}VisitedList;

typedef struct Frontier
{
	int* nodes_to_expand;//komvoi pros epektasi
	int size;//megethos pinaka komvwn
	int lelement;//teleftaio stoixeio
	int felement;//prwto stoixeio->gia epektasi
	int check;//epanatopothetisi stoixeiwn gia kaliteri ekmetalefsi mnimis->push left
}Frontier;



Frontier* frontier_init();
//VisitedList* visit_init();
int pop_frontier(Frontier*);
//void push_left(Frontier*);
//void visited_push(int, VisitedList*);
void push_frontier(int, Frontier*);
int expand(Frontier*, Path*, int, NodeIndex*, Buffer*,int);
//int visited_check(VisitedList*, int);
//void print_path(Path*,Path*);
int bidirectional(Frontier*, Path*, int, NodeIndex*, Buffer* ,Frontier* , Path* , int , NodeIndex*, Buffer*);
//int check_path(int, Path*);
int get_from_frontier(int, Frontier*);
int check_frontier(int, Frontier*);
int expand2(Frontier**, Frontier**, Frontier**, int, NodeIndex*, Buffer*,int);
int bidirectional2(int, NodeIndex*, Buffer*, int, NodeIndex*, Buffer*,int);
//int expand_cc(Frontier**, Frontier**, Frontier**, int, NodeIndex*, Buffer*, int);
int bidirectional_cc(int, NodeIndex*, Buffer*, int, NodeIndex*, Buffer*, int);
int expand_cc(Frontier**, NodeIndex*, NodeIndex*, Buffer*, Buffer*, int);
void create_cc(NodeIndex*, NodeIndex*, Buffer*, Buffer*,int*);

#endif