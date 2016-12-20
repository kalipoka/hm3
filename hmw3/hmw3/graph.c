/* graph.c */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "list.h"
#include "set.h"
#include "graph.h"

/************************DEFINITIONS**********************/
typedef struct _Graph* PGraph;

typedef struct _Graph
{
	PVertex_Set Vertex_set;         //not sure that enough parameters in the struct
	PEdge_Set Edge_set;	           //not sure that enough parameters in the struct, pointer to the 1st

} Graph, *PGraph;

typedef struct _Vertex_Set
{
	PVertex Vertex;
	/* Function Pointers */
	COMP_FUNC vertex_cmp;
	CLONE_FUNC vertex_clone;
	DESTROY_FUNC vertex_destroy;
} Vertex_Set, PVertex_Set;

typedef struct _Edge_Set
{
	PEdge Edge;
	/* Function Pointers */
	COMP_FUNC edge_cmp;
	CLONE_FUNC edge_clone;
	DESTROY_FUNC edge_destroy;
} Edge_Set, PEdge_Set;

/********************************FUNCTIONS*************************************/

/*****************************
*GraphCreate function
*The function creates the Graph
* Which has 2 sets: 1set of vertexes and 1 set of edges 
*allocates memory for the sets
*Argument ()
*Output (PGraph s)
*****************************/
PGraph GraphCreate()
{
	PGraph s;
	s = (PList)malloc(sizeof(Graph));            //Memory ALLOCITA
	if (s == NULL)
		return NULL;

	if (NULL == (s->Edge_set = SetCreate(cmp_edg, clone_edg, destroy_edg)))    //creates the 1st Edge (keshet)
		return NULL;

	if (NULL == (s->Vertex_set = SetCreate(cmp_ver, clone_ver, destroy_ver)))
		return NULL;

	return s;
}

/*****************************
*GraphAddVertex function
*The function add a vertex to the graph
*Argument (PGrapf s, int vertex_num)
*Output (TRUE/FALSE)
*****************************/

Bool GraphAddVertex(PGraph s, int vertex_num)
{
	PVertex new_vertex = (PVertex)malloc(sizeof(Vertex));

	/*Sanity Check*/
	if ((s == NULL) || (vertex_num < 0))   //check if this check is fine mayber need more
		return FALSE;
	
	new_vertex->serialNumber = vertex_num;

	if (SetAdd(s->Vertex_set, new_vertex) == FALSE) //adding the vertex to the Graph
		return FALSE;

	return TRUE;
}


Bool GraphAddEdge(PGraph pGraph, int vertex1, int vertex2, int weight);
PSet GraphNeighborVertices(PGraph, int);
Bool GraphFindShortestPath(PGraph pGraph, int source, int* dist, int* prev);

int GraphGetNumberOfEdges(PGraph);
int GraphGetNumberOfVertices(PGraph);

PSet GraphVerticesStatus(PGraph);
PSet GraphEdgesStatus(PGraph);
void GraphDestroy(PGraph)


/*********************************** Deeebugy & Testen ****************************************/
int main()
{
	PGraph tryingGraph;
	printf("Hola negritos Creating Graph\n");
	tryingGraph = GraphCreate();
	printf("The Graph was created DONE \n\n");

	printf("Adding Vertex Check &0 \n");
	Bool res1 = GraphAddVertex(tryingGraph, 0); // waiting for response from forum about adding zero Vertex
	printf("The Vertex &0 Was Added \n\n");

	printf("Adding Vertex Check &1 \n");
	Bool res2 = GraphAddVertex(tryingGraph, 1); // waiting for response from forum about adding zero Vertex
	printf("The Vertex &1 Was Added \n\n");

	return 0;
	
}