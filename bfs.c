#include <stdint.h>
#include "Bfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define STRLEN 1024


Frontier* frontier_init()
{
	Frontier* frontier = malloc(sizeof(Frontier));
	frontier->nodes_to_expand = malloc(10 * sizeof(int));
	frontier->size = 10;
	frontier->felement = 0;
	frontier->lelement = -1;
	frontier->check = 0;
	return frontier;
}

VisitedList* visit_init()
{
	int i;
	VisitedList* visited = malloc(sizeof(VisitedList));
	visited->path_array = malloc(10 * sizeof(int));
	visited->size = 10;
	visited->count = 0;
	for (i = 0; i < 10; i++) {
		visited->path_array[i] = -1;
	}
	return visited;
}

void fake_init(Frontier* frontier) {
	frontier->felement = 0;
	frontier->lelement = -1;
}

void push_frontier(int node, Frontier* frontier) {//eisagwgi komvwn pros epektasi
	if ((frontier->lelement+1) < frontier->size) {
		frontier->lelement++;
		frontier->nodes_to_expand[frontier->lelement] = node;

	}
	else {
		frontier->size = frontier->size * 2;
		frontier->nodes_to_expand = realloc(frontier->nodes_to_expand, frontier->size * sizeof(int));
		frontier->lelement++;
		frontier->nodes_to_expand[frontier->lelement] = node;
	}
}

int pop_frontier(Frontier* frontier) {
	int node_to_expand;
	int parent_node;
	if (frontier->lelement == -1 || frontier->felement>frontier->lelement) 
	{
		//printf("Adeia lista manmu");
	}
	else {
		node_to_expand = frontier->nodes_to_expand[frontier->felement];
		frontier->felement++;
		frontier->check++;
	//	printf("1");
		/*if (frontier->check == 5000) {//elegxos gia ekkinisi push left
			push_left(frontier);
			frontier->check = 0;
		}*/
		return frontier->nodes_to_expand[(frontier->felement)-1];
	}
	return -1;//kati stravwse
}



/*void visited_push(int node, VisitedList* visited_list) {//eisagwgi goniou kai komvou sto monopati
	if (visited_list->count < visited_list->size) 
	{
		visited_list->path_array[visited_list->count] = node;
		visited_list->count++;
	}
	else {
		visited_list->size = visited_list->size * 2;
		visited_list->path_array = realloc(visited_list->path_array, visited_list->size * sizeof(int));
		visited_list->path_array[visited_list->count] = node;
		visited_list->count++;
	}
}*/

/*int check_path(int node, Path* path) //elegxos an iparxei sto path
{
	int i=0;
	while (i < path->size) 
	{
		if (path->path_array[i].node == node) {
			return 1;
		}
		i++;
	}
	return 0;
}*/


int check_frontier(int node, Frontier* frontier)//elegxos an iparxei sto frontier
{
	int i = 0;
	while (i < frontier->lelement) {
		if (frontier->nodes_to_expand[i] == node) {
			return 1;
		}
		i++;
	}
	return 0;
}

int expand2(Frontier** main_frontier, Frontier** secondary_frontier, Frontier** main_frontier2, int target, NodeIndex* index, Buffer* buffer,int cycle) {
	int offset;
	int i = 0;
	int node;
	int flag = 0;
	Frontier* new_frontier;
	while (1)
	{
		node = pop_frontier(*main_frontier);
		if (node == -1&&flag==1)
		{
			new_frontier = *main_frontier;
			*main_frontier = *secondary_frontier;
			*secondary_frontier = new_frontier;
			return 0;
		}
		if (node == -1 && flag == 0)
		{
			return -1;
		}
		flag = 1;
		if (index->index_array[node].visited!=cycle) {//elegxos an einai visited se afto to kiklo tis bfs
			//visited_push(node, visited);
			index->index_array[node].visited = cycle;
			if (target == node || check_frontier(node,(*main_frontier2)))
			{
				return 1;
			}
			if (node < index->size)
			{//an den exei geitones kanei anadromi me neo pop
				offset = index->index_array[node].offset;
				i = 0;
				while (offset != -1)
				{
					while ((buffer->buffer_array[offset].emptyNeighborPos) > i)
					{//sarwsi geitonwn
						if (target == buffer->buffer_array[offset].neighbor[i])
						{
							return 1;
						}
						push_frontier(buffer->buffer_array[offset].neighbor[i], *secondary_frontier);
						i++;
					}
					offset = buffer->buffer_array[offset].nextListNode;
					i = 0;
				}
			}
		}
	}
}

int bidirectional2(int target1, NodeIndex* index, Buffer* buffer, int target2, NodeIndex* index2, Buffer* buffer2,int cycle) {
	int i = 0;
	int flag = 0;
	int temp;
	Frontier* main_frontier1;
	Frontier* main_frontier2;
	Frontier* temp_frontier;
	main_frontier1 = frontier_init();
	main_frontier2 = frontier_init();
	temp_frontier = frontier_init();
	push_frontier(target2, main_frontier1);
	push_frontier(target1, main_frontier2);
	while (1) {
		temp = expand2(&main_frontier1, &temp_frontier, &main_frontier2, target1, index, buffer,cycle);
		fake_init(temp_frontier);
		if (temp == 1)
		{
			return i;
		}
		else if (temp == -1)
		{
			if (flag == -1) {
				return -1;
			}
			else {
				flag = -1;
			}

		}
		else if (temp == 0) {
			flag = 0;
			i++;
		}
		temp = expand2(&main_frontier2, &temp_frontier, &main_frontier1, target2, index2, buffer2,cycle);
		fake_init(temp_frontier);
		if (temp == 1)
		{
			return i;
		}
		else if (temp == -1)
		{
			if (flag == -1) {
				return -1;
			}
			else {
				flag = -1;
			}

		}
		else if (temp == 0) {
			flag = 0;
			i++;
		}
	}
}




int bidirectional_cc(int target1, NodeIndex* index, Buffer* buffer, int target2, NodeIndex* index2, Buffer* buffer2,int cycle) {
	int i = 0;
	int flag = 0;
	int temp;
	Frontier* main_frontier1;
	Frontier* main_frontier2;
	Frontier* temp_frontier;
	main_frontier1 = frontier_init();
	main_frontier2 = frontier_init();
	temp_frontier = frontier_init();
	push_frontier(target2, main_frontier1);
	push_frontier(target1, main_frontier2);
	while (1) {
		temp = expand2(&main_frontier1, &temp_frontier, &main_frontier2, -1, index, buffer,cycle);
		fake_init(temp_frontier);
		if (temp == 1)
		{
			return i;
		}
		else if (temp == -1)
		{
			if (flag == -1) {
				return -1;
			}
			else {
				flag = -1;
			}

		}
		else if (temp == 0) {
			flag = 0;
			i++;
		}
		temp = expand2(&main_frontier2, &temp_frontier, &main_frontier1, -1, index2, buffer2,cycle);
		fake_init(temp_frontier);
		if (temp == 1)
		{
			return i;
		}
		else if (temp == -1)
		{
			if (flag == -1) {
				return -1;
			}
			else {
				flag = -1;
			}

		}
		else if (temp == 0) {
			flag = 0;
			i++;
		}
	}
}




int expand_cc(Frontier** main_frontier,NodeIndex* index, NodeIndex* index2, Buffer* buffer, Buffer* buffer2, int cycle) {
	int offset;
	int i = 0;
	int node;
	int flag = 0;
	Frontier* new_frontier;
	while (1)
	{
		node = pop_frontier(*main_frontier);
		if (node == -1)
		{
			return 1;
		}
		flag = 1;
		if (index->index_array[node].cc == -1) {//elegxos an einai visited se afto to kiklo tis bfs
														//visited_push(node, visited)
			index->index_array[node].cc = cycle;
			if (node < index->size)
			{//an den exei geitones kanei anadromi me neo pop
				offset = index->index_array[node].offset;
				i = 0;
				while (offset != -1)
				{
					while ((buffer->buffer_array[offset].emptyNeighborPos) > i)
					{//sarwsi geitonwn
						push_frontier(buffer->buffer_array[offset].neighbor[i], *main_frontier);
						i++;
					}
					offset = buffer->buffer_array[offset].nextListNode;
					i = 0;
				}
			}
			index2->index_array[node].cc = cycle;
			if (node < index->size)
			{//an den exei geitones kanei anadromi me neo pop
				offset = index2->index_array[node].offset;
				i = 0;
				while (offset != -1)
				{
					while ((buffer2->buffer_array[offset].emptyNeighborPos) > i)
					{//sarwsi geitonwn
						push_frontier(buffer2->buffer_array[offset].neighbor[i], *main_frontier);
						i++;
					}
					offset = buffer2->buffer_array[offset].nextListNode;
					i = 0;
				}
			}
		}
	}
}



void create_cc(NodeIndex* index1, NodeIndex* index2, Buffer* buffer1,Buffer* buffer2) {
	int i;
	int cc = 0;
	Frontier* main_frontier;
	Frontier* secondary_frontier;
	main_frontier = frontier_init();
	secondary_frontier = frontier_init();
	for (i = 0; i < index1->size; i++) {
		if (index1->index_array[i].cc == -1 && index1 -> index_array[i].offset != -1)
		{
			push_frontier(i, main_frontier);
			expand_cc(&main_frontier,index1,index2,buffer1,buffer2,cc);
			fake_init(main_frontier);
			cc++;
		}
	}
}





int tarjan()
{
	
}