#include "SCC.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#define STRLEN 1024
#define FRONTLEN 5
#define EDGELEN 3
#define N 10

SCC* SCC_create()
{
	SCC* scc;

	scc = malloc(sizeof(SCC));
	return scc;
}

void SCC_initialize(SCC* SCC_ptr, int array_length, NodeIndex* index)
{
	int i;
	int indexSize;
	int *invertedIndex;
	Component *componentIndex;

	componentIndex = malloc(sizeof(Component)*array_length);
	SCC_ptr->size = array_length;
	SCC_ptr->components = componentIndex;

	indexSize = index->size;
	invertedIndex = malloc(sizeof(int) * indexSize);
	for (i = 0; i<indexSize; i++) {
		invertedIndex[i] = -1;
	}
	SCC_ptr->id_belongs_to_component = invertedIndex;
	SCC_ptr->components_count = 0;

}

Front* front_create()
{
	Front* front;
	front = malloc(sizeof(Front));
	front->front_array = malloc(FRONTLEN * sizeof(int));
	front->size = FRONTLEN;
	front->last = -1;
	return front;
}

Stack* stack_create()
{
	Stack* stack;
	stack = malloc(sizeof(Front));
	stack->stack_array = malloc(FRONTLEN * sizeof(int));
	stack->size = FRONTLEN;
	stack->last = -1;
	stack->current = 0;
	return stack;
}

void push_frontier_tarjan(int node, Front* frontier)
{
	if ((frontier->last + 1) < frontier->size)
	{
		frontier->last++;
		frontier->front_array[frontier->last] = node;

	}
	else
	{
		frontier->size = frontier->size * 2;
		frontier->front_array = realloc(frontier->front_array, frontier->size * sizeof(int));
		frontier->last++;
		frontier->front_array[frontier->last] = node;
	}
}


int pop_frontier_tarjan(Front* frontier)
{

	if (frontier->last == -1)
	{
		printf("Adeia lista manmu \n");
		return -1;
	}
	else
	{
		frontier->last--;
		return frontier->front_array[(frontier->last) + 1];
	}
}



void SCC_update_solo(SCC *scc, int node)
{
	int comp_count;

	comp_count = scc->components_count;
	if (comp_count == scc->size)
	{
		scc->size = 2 * scc->size;
		scc->components = realloc(scc->components, scc->size*sizeof(Component));
		if (scc->components == NULL)
		{
			printf("Null realloc \n");
			return;
		}
	}
	//	printf("O %d den exei geitones.Einai sunistwsa monh ths!\n",i);
	scc->components[comp_count].component_id = comp_count;
	scc->components[comp_count].included_nodes_count = 1;
	scc->components[comp_count].included_node_ids = malloc(sizeof(int));
	//printf("boop \n");
	if (scc->components[comp_count].included_node_ids == NULL)
	{
			printf("Gia ton poutso PC sxolhs \n");
		return;
	}
	scc->components[comp_count].included_node_ids[0] = node;

	scc->id_belongs_to_component[node] = comp_count;
	scc->components_count++;
}


void SCC_update(Stack* stack,SCC *scc,NodeIndex* index, int fin_node ,Position* pos_array)
{
	int comp_count, prime_lowlink, temp_node,temp_lowlink;
	temp_node = check_stack_tarjan(stack);
	prime_lowlink = pos_array[temp_node].lowlink;
	temp_lowlink = prime_lowlink;
	comp_count=scc->components_count;
	if (scc->components_count == scc->size)
	{
		scc->size = 2 * scc->size;
		//printf("trying to allocate this size : %d \n", scc->size);
		//printf("boop 8\n");
		if (scc->components == NULL) {
			printf("Fuck \n");
		}

		scc->components = realloc(scc->components, scc->size * sizeof(Component));
		//printf("boop2 \n");
		if (scc->components == NULL)
		{
			printf("Null realloc \n");
			return;
		}
	}
	scc->components[comp_count].included_nodes_count = 0;
	scc->components[comp_count].included_node_ids = malloc(sizeof(int) * 10);
	scc->components[comp_count].array_size=10;
	do {
		//printf("boop 7 \n");
		scc->components[comp_count].included_nodes_count++;
		temp_node = pop_stack_tarjan(stack);
		pos_array[temp_node].instack = 0; //to vgazoume apo ti lista
		if (scc->components[comp_count].included_nodes_count == scc->components[comp_count].array_size)
		{
			scc->components[comp_count].array_size = scc->components[comp_count].array_size * 2;
			scc->components[comp_count].included_node_ids = realloc(scc->components[comp_count].included_node_ids, scc->components[comp_count].array_size*sizeof(int));
		}
		scc->components[comp_count].component_id = comp_count;
		//printf("boop  3 \n");
		/*if (scc->components[comp_count].included_nodes_count == scc->ids_size) //me+1 sta aristera nomizw pio apodotiko
		{
			scc->ids_size = scc->ids_size * 2;
			scc->id_belongs_to_component=realloc(scc->id_belongs_to_component, scc->ids_size * sizeof(int));
		}*/
		if (scc->components[comp_count].included_node_ids == NULL)
		{
			printf("Gia ton poutso PC sxolhs \n");
			return;
		}
		scc->components[comp_count].included_node_ids[(scc->components[comp_count].included_nodes_count)-1] = temp_node;
		scc->id_belongs_to_component[temp_node] = comp_count;
		//printf("boop 4 \n");
		//temp_node = check_stack_tarjan(stack);
		//temp_lowlink = index->index_array[temp_node].lowlink;
	} while (temp_node != fin_node);
	scc->components_count++;
	return;
}


int expand_tarjan(int node, NodeIndex* index, Buffer* buffer,SCC* scc,Position* pos_array)
{
	int expandedNodes = 0;
	int i, neighbor, nextNode, temp_node2;
	int temp_node,temp_neigh;
	int temp_scc=0;
	int break_check = 0;
	int num = 0;
	Stack* stack;
	Front* frontier;
	indexStruct* index_array = index->index_array;
	list_node* buffer_array = buffer->buffer_array;
	int offset = index_array[node].offset;
	nextNode = buffer_array[offset].nextListNode;
	stack = stack_create();
	//frontier = front_create();
	//push_frontier_tarjan(node, frontier);
	push_stack_tarjan(node, stack);
	pos_array[node].lowlink = ++temp_scc;
	pos_array[node].uplink = temp_scc;
	pos_array[node].tarjan_visited = 1;
	pos_array[node].instack = 1;
	pos_array[node].caller = -1;
	temp_node = node;
	if (temp_node == -1) {
		printf("BOom");
	}
	//while ((temp_node = check_stack_tarjan(stack)) != -1) 
	while(true)
	{
		/*if (index->index_array[temp_node].offset == -1) 
		{
			SCC_update_solo(scc, temp_node);//an den exei pou na epektathei
			pop_stack_tarjan(stack);
			index->index_array[temp_node].instack = 0;
			temp_node2 = index->index_array[temp_node].caller;
			if (index->index_array[temp_node2].caller != -1) {
				if (index->index_array[temp_node2].lowlink > index->index_array[temp_node].lowlink) {
					index->index_array[temp_node2].lowlink = index->index_array[temp_node].lowlink;
				}
				temp_node = temp_node2;
			}
			else
			{
				break;
			}
		}
		else 
		{*/
				i = pos_array[temp_node].node;
				offset = pos_array[temp_node].offset;
				break_check = 0;
				/*if (temp_node == -1) {
					printf("BOom");
				}*/
				if((offset!=-1) && ((pos_array[temp_node].node < buffer->buffer_array[offset].emptyNeighborPos) || (buffer->buffer_array[offset].nextListNode != -1))) 
				{//anaptiksi geitonwn komvou
					
					if (pos_array[temp_node].node == buffer->buffer_array[offset].emptyNeighborPos) {
						pos_array[temp_node].offset = buffer->buffer_array[offset].nextListNode;
						pos_array[temp_node].node = 0;
						offset = pos_array[temp_node].offset;
					}
					temp_neigh = buffer->buffer_array[offset].neighbor[pos_array[temp_node].node];
					pos_array[temp_node].node++;
					if (pos_array[temp_neigh].tarjan_visited != 1) {
						push_stack_tarjan(temp_neigh, stack);
						pos_array[temp_neigh].instack = 1;
						pos_array[temp_neigh].tarjan_visited = 1;//visited idiou kiklou?
						pos_array[temp_neigh].lowlink = ++temp_scc;
						pos_array[temp_neigh].uplink = temp_scc;
						pos_array[temp_neigh].caller = temp_node;
						temp_node = temp_neigh;
						/*if (temp_node == -1) {
							printf("BOom");
						}*/
						/*break_check = 1;
						break;*/
					}
					else if(pos_array[temp_neigh].instack == 1){
						//SCC_update(stack,scc,index,temp_neigh);
						if (pos_array[temp_node].lowlink > pos_array[temp_neigh].uplink) {
							pos_array[temp_node].lowlink = pos_array[temp_neigh].uplink;
						}
						/*break_check = 1;
						break;*/
					}
				
				/*if (break_check == 1) {
					break;
				}*/
			} //while (pos_array[temp_node].offset != -1);
		/*}*/
				else {
					if (pos_array[temp_node].lowlink == pos_array[temp_node].uplink) {
						SCC_update(stack, scc, index, temp_node, pos_array);
						//temp_node = check_stack_tarjan(stack);
					}
				/*if (temp_node == -1) {
					printf("BOom");
				}
				//printf("Creating Scc num %d",num++);
			}
			if (temp_node == -1) {
				printf("BOom");
			}*/
			temp_node2 = pos_array[temp_node].caller;
			/*if (temp_node == -1) {
				printf("BOom");
			}
			if (temp_node2 == -1) {
				printf("BOom");
			}*/
			if (temp_node == -1) {
				break;
			}
			if (temp_node2 != -1) {
				if (pos_array[temp_node2].lowlink > pos_array[temp_node].lowlink) {
					pos_array[temp_node2].lowlink = pos_array[temp_node].lowlink;
				}
				temp_node = temp_node2;
				/*if (temp_node == -1) {
					printf("BOom");
				}*/
			}
			else 
			{
				break; 
			}
		}
	}
	/*while (nextNode != -1)
	{
		for (i = 0; i < buffer_array[offset].emptyNeighborPos; i++)
		{

			neighbor = buffer_array[offset].neighbor[i];
			if (index_array[neighbor].tarjanvisited == 1)
				continue;
			else
			{
				expandedNodes++;
				push_frontier_tarjan(neighbor, frontier);
			}
		}
		offset = nextNode;
		nextNode = buffer_array[offset].nextListNode;
	}

	int tail = index_array[node].tail;
	for (i = 0; i < buffer_array[tail].emptyNeighborPos; i++)
	{
		neighbor = buffer_array[tail].neighbor[i];
		if (index_array[neighbor].tarjanvisited == 1)
			continue;
		else
		{
			expandedNodes++;
			push_frontier_tarjan(neighbor, frontier);

		}
	}*/
	

}


void push_stack_tarjan(int node, Stack* stack)
{
	if ((stack->last + 1) < (stack->size))
	{
		stack->last++;
		stack->stack_array[stack->last] = node;

	}
	else
	{
		stack->size = stack->size * 2;
		stack->stack_array = realloc(stack->stack_array, stack->size * sizeof(int));
		stack->last++;
		stack->stack_array[stack->last] = node;
	}
}


int check_stack_tarjan(Stack* stack) {
	if (stack->last == -1)
	{
		printf("Adeia lista manmu \n");
		return -1;
	}
	else
	{
		return stack->stack_array[stack->last];
	}
}

int pop_stack_tarjan(Stack* stack)
{
	if (stack->last == -1)
	{
		printf("Adeia lista manmu \n");
		return -1;
	}
	else
	{
		stack->last--;
		return stack->stack_array[(stack->last) + 1];
	}
}


void tarjan(NodeIndex* index, NodeIndex* index2, Buffer* buffer, Buffer* buffer2, SCC* scc)
{
	indexStruct* index_array = index->index_array;
	list_node* buffer_array = buffer->buffer_array;
	indexStruct* index_array2 = index2->index_array;
	list_node* buffer_array2 = buffer2->buffer_array;

	int Index = 1;
	int i = 0;
	int offset, offset2, comp_count;
	int temp_scc = 0;
	comp_count=scc->components_count;
	Front* frontier = front_create();
	Stack* stack = stack_create();
	Position* pos_array = malloc(sizeof(Position)*index->size);
	for (i = 0; i < index->size; i++) {
		pos_array[i].node = 0;
		pos_array[i].offset = index->index_array[i].offset;
		pos_array[i].instack=-1;
		pos_array[i].tarjan_visited=-1;
		pos_array[i].caller = -1;

	}
	for (i = 0; i < index->size; i++)
	{	
		offset = index->index_array[i].offset;
		offset2 = index2->index_array[i].offset;
		if ((offset == -1) && (offset2 == -1))
		{
			//printf("Den uparxei o komvos %d! \n",i);
			continue;
		}

		if (pos_array[i].tarjan_visited == 1)
		{
			continue;
		}

		comp_count = scc->components_count;
		offset = index_array[i].offset;
		offset2 = index_array2[i].offset;
		if (offset == -1)
		{
			SCC_update_solo(scc, i);
		}
		// printf("Geitones tou %d : %d \n",i,buffer_array[offset].emptyNeighborPos);
		/*if ((offset == -1) && (offset2 != -1))
		{
			if (scc->components_count == scc->size)
			{
				scc->size = 2 * scc->size;
				scc->components = realloc(scc->components, scc->size);//WTF SIZE
				if (scc->components == NULL)
				{
					printf("Null realloc \n");
					return;
				}
			}
		
			//	printf("O %d den exei geitones.Einai sunistwsa monh ths!\n",i);
			scc->components[comp_count].component_id = comp_count;
			scc->components[comp_count].included_nodes_count = 1;
			scc->components[comp_count].included_node_ids = malloc(sizeof(int));
			if (scc->components[comp_count].included_node_ids == NULL)
			{

				//	printf("Gia ton poutso PC sxolhs \n");
				return;
			}
			scc->components[comp_count].included_node_ids[0] = i;

			scc->id_belongs_to_component[i] = comp_count;
			scc->components_count++;

			pos_array[i].tarjan_visited = 1;

		}*/
		else if (offset != -1)
		{


			//push_frontier_tarjan(i, frontier);
			expand_tarjan(i, index, buffer,scc,pos_array);

		}

	}
	return;
}

QueryComp* edge_table(QueryComp* ptr)
{

	int i;
	ptr = malloc(sizeof(QueryComp));
	ptr->table = malloc(EDGELEN * sizeof(int*));

	for (i = 0; i < EDGELEN; i++)
	{
		ptr->table[i] = malloc(2 * sizeof(int));
	}
	ptr->size = EDGELEN;
	ptr->current = -1;
	return ptr;
}

void add_comp_edge(int cc1, int cc2, QueryComp* ptr)
{
	int pos;
	if (ptr->current == (ptr->size) - 1)
	{

		//		printf("Tha kanei realloc kapote \n");
		ptr->size = (2 * ptr->size);
		//		printf("SIZEEEE 1 | %d |.",ptr->size);

		ptr->table = realloc(ptr->table, (ptr->size) * sizeof(int*));
		for (pos = ptr->current + 1; pos < (ptr->size); pos++)
		{
			ptr->table[pos] = malloc(2 * sizeof(int));
		}
		//		printf("Kanw relloc xazouli \n");
	}
	//	printf("Klaiw \n");
	//	printf("SIZEEEE 2 | %d |.",(ptr->size) ) ;
	ptr->table[(ptr->current) + 1][0] = cc1;
	ptr->table[(ptr->current) + 1][1] = cc2;
	ptr->current = ptr->current + 1;
	//	print_edge_table(ptr);

}

void print_edge_table(QueryComp* ptr)
{
	int i, length;
	length = ptr->size;
	int** table = ptr->table;
	printf("|==================================================| \n");
	for (i = 0; i< ptr->current; i++)
	{
		printf("Array[%d][0]= | %d | \n", i, table[i][0]);
		printf("Array[%d][1]= | %d | \n ", i, table[i][1]);
		printf("\n");
	}
	printf("|==================================================| \n");
}


void weekly(int node1, int node2, NodeIndex* index, NodeIndex* index2, int* new_component, QueryComp* ptr)
{
	//	printf("CC1: | %d | . CC2 | %d | \n",index_array[node1].cc, index_array2[node2].cc);
	//	printf("Node1:| %d | Node2: | %d | \n",node1,node2);
	//printf("CC1: | %d | . CC2 | %d | \n",index_array[node1].cc, index_array2[node2].cc);

	indexStruct* index_array = index->index_array;
	indexStruct* index_array2 = index2->index_array;

	//	printf("CC1: | %d | . CC2 | %d | \n",index_array[node1].cc, index_array2[node2].cc);

	//printf("Molis arxikopoihsa \n");
	if ((index_array[node1].cc == -1) && (index_array2[node2].cc == -1))
	{
		//		printf("1 \n");
		index_array[node1].cc = (*new_component);
		index_array2[node2].cc = (*new_component);
		(*new_component)++;

	}
	else if ((index_array[node1].cc != -1) && (index_array2[node2].cc == -1))
	{
		//		printf("2 \n");
		index_array[node1].cc = index_array2[node2].cc;
	}
	else if ((index_array[node1].cc == -1) && (index_array2[node2].cc != -1))
	{
		//		printf("3 \n");
		index_array2[node2].cc = index_array[node1].cc;
	}
	else if ((index_array[node1].cc != -1) && (index_array2[node2].cc != -1))
	{
		if (index_array[node1].cc == index_array2[node2].cc)
		{
			return;
		}
		//		printf("Paw na addarw sto pinakaki \n");
		add_comp_edge(index_array[node1].cc, index_array2[node2].cc, ptr);
	}
}







//sinartisi apo git
/*
*d[i] = Discovery time of node i. (Initialize to -1)
*low[i] = Lowest discovery time reachable from node
i. (Doesn't need to be initialized)
*scc[i] = Strongly connected component of node i. (Doesn't
need to be initialized)
*s = Stack used by the algorithm (Initialize to an empty
stack)
*stacked[i] = True if i was pushed into s. (Initialize to
false)
*ticks = Clock used for discovery times (Initialize to 0)
*current_scc = ID of the current_scc being discovered
(Initialize to 0)*/
/*stack<int> s;
bool stacked[MAXN];
vector<int> g[MAXN];
int d[MAXN], low[MAXN], scc[MAXN];
bool stacked[MAXN];
stack<int> s;
int ticks, current_scc;
void tarjan(int u) {
	d[u] = low[u] = ticks++;
	s.push(u);
	stacked[u] = true;
	const vector<int> &out = g[u];
	for (int k = 0, m = out.size(); k<m; ++k) {
		const int &v = out[k];
		if (d[v] == -1) {
			tarjan(v);
			low[u] = min(low[u], low[v]);
		}
		else if (stacked[v]) {
			low[u] = min(low[u], low[v]);
		}
	}
	if (d[u] == low[u]) {
		int v;
		do {
			v = s.top();
			s.pop();
			stacked[v] = false;
			scc[v] = current_scc;
		} while (u != v);
		current_scc++;
	}
}*/





