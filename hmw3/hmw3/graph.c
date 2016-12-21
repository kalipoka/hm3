/* graph.c */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "list.h"
#include "set.h"
#include "graph.h"

/************************DEFINITIONS**********************/
struct _Graph;
typedef struct _Graph{
	PSet Vertex_set;         //not sure that enough parameters in the struct
	PSet Edge_set;	           //not sure that enough parameters in the struct, pointer to the 1st

} Graph;

/********************************HELPERS***************************************/

static void print_vertex_list(PGraph s)
{
	PElem elem = SetGetFirst(s->Vertex_set);
	printf("\nverteces in list:\n");
	while (elem)
	{
		PVertex ver = (PVertex)elem;
		printf("%d\n", ver->serialNumber);
		elem = SetGetNext(s->Vertex_set);
	}
}

static void print_edges_list(PGraph s)
{
	PElem elem = SetGetFirst(s->Edge_set);
	printf("\nedges in list:\n");
	while (elem)
	{
		PEdge ver = (PEdge)elem;
		printf("{%d, %d}\n", ver->nodeA->serialNumber, ver->nodeB->serialNumber, ver->weight);
		elem = SetGetNext(s->Edge_set);
	}
}


static PElem find_vertex(PGraph s, PElem pElem)
{
	return SetFindElement(s->Vertex_set, pElem);
}

static PElem find_vertex_by_num(PGraph s, int serial)
{
	PElem elem = SetGetFirst(s->Vertex_set);
	
	while (elem)
	{
		PVertex ver = (PVertex)elem;
		if (ver->serialNumber == serial)
			return elem;
		elem = SetGetNext(s->Vertex_set);
	}
	return NULL;
}

static PElem find_edge(PGraph s, PElem pElem)
{
	return SetFindElement(s->Edge_set, pElem);
}

static void destroy_ver(PElem pElem)
{
	PVertex pVer = (PVertex)pElem;
	free(pVer);
}

static void destroy_edg(PElem pElem)
{
	PEdge pEdge = (PEdge)pElem;
	free(pEdge);
}

static Bool cmp_edg(PElem pElem1, PElem pElem2) {
	if (!pElem1 || !pElem2) return FALSE;
	PEdge pEdge1 = (PEdge)pElem1;
	PEdge pEdge2 = (PEdge)pElem2;

	if (((pEdge1->nodeA == pEdge2->nodeA) && (pEdge1->nodeB == pEdge2->nodeB)) || ((pEdge1->nodeA == pEdge2->nodeB) && (pEdge1->nodeB == pEdge2->nodeA)))
	return FALSE;
}

static Bool cmp_ver(PElem pElem1, PElem pElem2) {
	if (!pElem1 || !pElem2) return FALSE;
	PVertex pVer1 = (PVertex)pElem1;
	PVertex pVer2 = (PVertex)pElem2;

	if (pVer1->serialNumber == pVer2->serialNumber)
			return TRUE;
	return FALSE;
}

static PElem clone_edg(PElem pElem)
{
	if (!pElem) return NULL; 
	PEdge pEdge_src = (PEdge) pElem;
	PEdge pEdge_new;
	pEdge_new =(PEdge)malloc(sizeof(Edge));
	if (!pEdge_new) return NULL; // ALLOC CHECK

	pEdge_new->nodeA = pEdge_src->nodeA;
	pEdge_new->nodeB = pEdge_src->nodeB;
	pEdge_new->weight = pEdge_src->weight;

	return pEdge_new;
}

static PElem clone_ver(PElem pElem)
{
	if (!pElem) return NULL;
	PVertex pVertex_src = (PVertex)pElem;
	PVertex pVertex_new;
	pVertex_new = (PVertex)malloc(sizeof(Vertex));
	if (!pVertex_new) return NULL; // ALLOC CHECK

	pVertex_new->serialNumber = pVertex_src->serialNumber;
	return pVertex_new;
}

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
	s = (PGraph)malloc(sizeof(Graph));            //Memory ALLOCITA
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
	/*Sanity Check*/
	if ((s == NULL) || (vertex_num < 0))   //check if this check is fine mayber need more
		return FALSE;
	
	if (find_vertex_by_num(s, vertex_num)) //if we've found a similar vertex
		return FALSE;

	PVertex new_vertex = (PVertex)malloc(sizeof(Vertex));
	if (!new_vertex) return FALSE;
	new_vertex->serialNumber = vertex_num;

	if (SetAdd(s->Vertex_set, new_vertex) == FALSE) //adding the vertex to the Graph
		return FALSE;

	return TRUE;
}

Bool GraphAddEdge(PGraph s, int vertex1, int vertex2, int weight)
{
	/*Sanity Check*/
	if ((s == NULL) || (vertex1 < 0) || (vertex2 < 0) || (weight < 0) || (weight>10) )   //check if this check is fine mayber need more
		return FALSE;

	PEdge new_edge = (PEdge)malloc(sizeof(Edge));
	if (!new_edge) return FALSE;

	new_edge->nodeA = find_vertex_by_num(s, vertex1);
	new_edge->nodeB = find_vertex_by_num(s, vertex2);
	if ((!new_edge->nodeA) || (!new_edge->nodeB) || (new_edge->nodeA == new_edge->nodeB)) // checks if the nodes exist in the graph
	{
		free(new_edge); //some node doesn't exist
		return FALSE;
	}
	new_edge->weight = weight;

	if (find_edge(s, new_edge)) //if we've found a similar edge
	{
		free(new_edge);
		return FALSE;
	}

	if (SetAdd(s->Edge_set, new_edge) == FALSE) //adding the vertex to the Graph
		return FALSE;

	return TRUE;
}

PSet GraphNeighborVertices(PGraph, int);

Bool GraphFindShortestPath(PGraph pGraph, int source, int* dist, int* prev);

/*****************************
*GraphGetNumberOfEdges function
*The function return the number of exisiting edges in the graph
*Argument (PGraph s)
*Output (integer)
*****************************/
int GraphGetNumberOfEdges(PGraph s)
{
	if (!s) return 0;
	return SetGetSize(s->Edge_set);
}

/*****************************
*GraphGetNumberOfVertices function
*The function return the number of exisiting Vertices in the graph
*Argument (PGraph s)
*Output (integer)
*****************************/
int GraphGetNumberOfVertices(PGraph s)
{
	if (!s) return 0;
	return SetGetSize(s->Vertex_set);
}

/*****************************
*GraphVerticesStatus function
*The function returns a pointer to the vertex set
*Argument (PGraph s)
*Output (PSet s)
*****************************/
PSet GraphVerticesStatus(PGraph s)
{
	if (!s) return NULL;
	return s->Vertex_set;
}

/*****************************
*GraphEdgesStatus function
*The function returns a pointer to the edges set
*Argument (PGraph s)
*Output (PSet s)
*****************************/
PSet GraphEdgesStatus(PGraph s)
{
	if (!s) return NULL;
	return s->Edge_set;
}

/*****************************
*GraphDestroy function
*The function destroys the two sets in the the grapsh and the graph itself
*Argument (PGraph s)
*No output
*****************************/
void GraphDestroy(PGraph s)
{
	SetDestroy(s->Edge_set);
	SetDestroy(s->Vertex_set);
	free(s);
}


/*********************************** Deeebugy & Testen ****************************************/
int main()
{
	PGraph tryingGraph;
	printf("Hola negritos Creating Graph\n");
	tryingGraph = GraphCreate();
	printf("The Graph was created DONE \n\n");
	
	printf("Adding Vertex Check &0 \n");
	Bool res1 = GraphAddVertex(tryingGraph, 0); // waiting for response from forum about adding zero Vertex
		if (res1 == FALSE)
			printf("Adding Vertex Check &0 FAILED \n\n");

	print_vertex_list(tryingGraph);

	printf("Adding Vertex Check &1 \n");
	Bool res2 = GraphAddVertex(tryingGraph, 1); // waiting for response from forum about adding zero Vertex
	if (res2 == FALSE)
		printf("Adding Vertex Check &1 FAILED \n\n");

	print_vertex_list(tryingGraph);

	printf("Adding Vertex Check &2 \n");
	Bool res6 = GraphAddVertex(tryingGraph, 2); // waiting for response from forum about adding zero Vertex
	if (res6 == FALSE)
		printf("Adding Vertex Check &2 FAILED \n\n");
	
	print_vertex_list(tryingGraph);


	printf("Adding Edge Check &0 \n");
	Bool res3 = GraphAddEdge(tryingGraph, 0, 1, 3);
	if (res3 == FALSE)
		printf("Adding Edge Check &0 FAILED \n\n");

	print_edges_list(tryingGraph);

	printf("Adding Edge Check &1 \n");
	Bool res4 = GraphAddEdge(tryingGraph, 0, 2, 3);
	if (res4 == FALSE)
		printf("Adding Edge Check &1 FAILED - cause you are fucking nigha \n\n");

	print_edges_list(tryingGraph);
	/*
	printf("Adding Edge Check &2 \n");
	Bool res5 = GraphAddEdge(tryingGraph, 1, 2, 3);
	if (res5 == FALSE)
		printf("Adding Edge Check &2 FAILED \n\n");
    */
	printf("number of vertex %d \n\n", GraphGetNumberOfVertices(tryingGraph));

	printf("number of Edges %d \n\n", GraphGetNumberOfEdges(tryingGraph));


	print_edges_list(tryingGraph);

	GraphDestroy(tryingGraph);

	printf("number of vertex %d \n\n", GraphGetNumberOfVertices(tryingGraph));

	printf("number of Edges %d \n\n", GraphGetNumberOfEdges(tryingGraph));


	return 0;	
}