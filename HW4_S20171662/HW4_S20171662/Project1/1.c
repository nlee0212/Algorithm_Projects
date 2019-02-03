#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NAMELEN 100

char commandfile[NAMELEN] = "commands.txt";
char openfile[NAMELEN];
char resultfile[NAMELEN];

FILE* of;
FILE* rf;

int max_weight = 0;
//e: index of the 'result' array at KruskalMST()
int e = 0;

time_t litstart = 0, litend = 0;

//saves the information of each component (parent vertex, total weight, total number of vertices)
typedef struct comp_ {
	int parent_id;
	__int64 weight;
	int vertices;
}component;

//saves the information of each edge - the source vertex, the destination vertex, and its weight
typedef struct edge_ {
	int src, dest, weight;
} Edge;

//used in KruskalMST()
//used to save the edges chosen when performing the algorithm
Edge* result;

//saves the information of the whole graph, with disjoint sets
typedef struct graph_ {
	int V, E;
	Edge* edge;
}Graph;

//creates a graph, saves the information of the number of vertices and edges, and each of the edges given
Graph* createGraph(int V, int E) {
	Graph* graph = (Graph*)malloc(sizeof(Graph));

	graph->V = V;
	graph->E = E;
	graph->edge = (Edge*)malloc((graph->E) * sizeof(Edge));

	return graph;
}

//used to save each vertex's parent and rank
typedef struct subset_ {
	int parent;
	int rank;
}subset;

subset* subsets;

//a function to find the given vertex's parent
int find(subset subsets[], int i) {
	if (subsets[i].parent != i)
		subsets[i].parent = find(subsets, subsets[i].parent);

	return subsets[i].parent;
}

//compares each vertex's parent, and merge the two trees
//the tree with a smaller rank is set to be the child tree of the tree with a bigger rank
//if the two trees' ranks are the same, then set the first tree's root to be the second tree's parent
void Union(subset subsets[], int x, int y) {
	int xroot = find(subsets, x);
	int yroot = find(subsets, y);

	if (subsets[xroot].rank < subsets[yroot].rank)
		subsets[xroot].parent = yroot;

	else if (subsets[xroot].rank > subsets[yroot].rank)
		subsets[yroot].parent = xroot;

	else {
		subsets[yroot].parent = xroot;
		subsets[xroot].rank++;
	}
}

//used in qsort()
//compare the two weights and return subtraction of the two
int myComp(const void* a, const void* b) {
	Edge* a1 = (Edge*)a;
	Edge* b1 = (Edge*)b;

	return a1->weight - b1->weight;
}

//THE MAIN FUNCTION OF THIS PROGRAM - finding the MST using Kruskal's Algorithm
void KruskalMST(Graph* graph) {
	//saving the num of the vertex of this graph
	int V = graph->V;

	//create the resulting minimum spanning tree, having each element being the 'Edge'
	result = (Edge*)malloc(sizeof(Edge)*(V - 1));

	//initializing
	//i: index of the 'graph->edge' array
	int i = 0;
	int v;
	int x, y;

	//used as temporary edge to save the information of the current edge
	Edge next_edge;

	litstart = clock();
	//sorting the array of edges with non-decreasing order considering each of their weight
	qsort(graph->edge, graph->E, sizeof(graph->edge[0]), myComp);
	litend = clock();

	//creating V subsets
	subsets = (subset*)malloc(sizeof(subset)*V);

	//initialization of the subsets - setting the parent to itself, and the rank to 0
	for (v = 0;v < V;++v) {
		subsets[v].parent = v;
		subsets[v].rank = 0;
	}

	//the number of edges to be taken has to be equal to V-1
	while (e < V - 1) {
		//picking the smallest edge and increasing the index for iteration
		//next edge's weight would be bigger or equal to the previous edge's weight
		if (i == graph->E)
			break;
		
		next_edge = graph->edge[i++];

		//then we find the parent of each source and destination vertex
		x = find(subsets, next_edge.src);
		y = find(subsets, next_edge.dest);

		//if the parents are different, it means that they won't form a cycle when merged
		//so merge them!
		if (x != y) {
			result[e++] = next_edge;
			Union(subsets, x, y);
		}
	}
}

int main() {
	int V;
	int E;
	int i,j;
	int src;
	//compnum: num of components
	int compnum = 0;

	//read command.txt, read the file names to open and to write the result
	FILE*fp = fopen(commandfile, "r");
	fscanf(fp, "%s", openfile);
	fscanf(fp, "%s", resultfile);
	fclose(fp);

	//read the data file to get the information of the number of vertices, edges, maximum weight
	of = fopen(openfile, "r");
	fscanf(of, "%d", &V);
	fscanf(of, "%d", &E);
	fscanf(of, "%d", &max_weight);

	//create Graph using V & E
	Graph* graph = createGraph(V, E);

	//save the information of each of the edges
	for (i = 0;i < E;i++)
	{
		fscanf(of, "%d", &graph->edge[i].src);
		fscanf(of, "%d", &graph->edge[i].dest);
		fscanf(of, "%d", &graph->edge[i].weight);

		if (graph->edge[i].weight > max_weight)
		{
			printf("Edge's weight too big!!\n");
			return 0;
		}
	}

	fclose(of);

	//open the file to write the results
	rf = fopen(resultfile, "w");

	time_t start = 0, end = 0;
	start = clock();
	//perform Kruskal's algorithm by using the idea of a minimum spanning tree
	KruskalMST(graph);
	end = clock();

	//generate an array composed of V 'component's
	component* components = (component*)malloc(sizeof(component)*V);

	//find the number of components by finding a vertex who's its parent is itself - meaning that it is a root of a set
	//the number of roots == the number of components
	for (i = 0;i < V-1;i++)
	{
		if (subsets[i].parent == i)
		{
			components[compnum].parent_id = i;
			compnum++;
		}
	}

	//print the number of components on the result file
	fprintf(rf, "%d\n", compnum);

	//initialize 'component's weight and vertices
	//vertices is set to 1 because it already has a vertex - the root
	for (i = 0;i < compnum;i++)
	{
		components[i].weight = 0;
		components[i].vertices = 1;
	}

	//go through the chosen edges to gain the total number of vertices and the total weight of each of the connected components
	for (i = 0;i < e;++i)
	{
		//find the edge's source's parent
		src = find(subsets, result[i].src);

		//then add its weight to the component that stands for its root vertex
		//then add 1 to the vertex of the component
		for (j = 0;j < compnum;j++)
		{
			if (src == components[j].parent_id)
			{
				components[j].weight += result[i].weight;
				components[j].vertices++;
				break;
			}
		}
	}

	//print each of the components' number of vertices and total weight
	for (i = 0;i < compnum;i++)
		fprintf(rf, "%d %I64d\n", components[i].vertices, components[i].weight);

	float first, second;
	first =(float) (end - start) / (CLOCKS_PER_SEC);
	second = (float)(litend - litstart) / (CLOCKS_PER_SEC);

	printf("\n전체 수행 시간: %f\nsorting 수행 시간: %f\n", first , second );

	free(graph);
	free(components);
	free(result);
	free(subsets);
	fclose(rf);

	return 0;
}