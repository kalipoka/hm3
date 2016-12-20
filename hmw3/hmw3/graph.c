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
	PSet Vertex_set;         //not sure that enough parameters in the struct
	PSet Edge_set;	           //not sure that enough parameters in the struct, pointer to the 1st

} Graph, *PGraph;

/********************************HELPERS***************************************/

static DESTROY_FUNC destroy_ver(PElem pElem)
{
	PVertex pVer = (PVertex)pElem;
	free(pVer);
}

static DESTROY_FUNC destroy_edg(PElem pElem)
{
	PEdge pEdge = (PEdge)pElem;
	free(pEdge);
}

static COMP_FUNC cmp_edg(PElem pElem1, PElem pElem2) {
	if (!pElem1 || !pElem2) return FALSE;
	PEdge pEdge1 = (PEdge)pElem1;
	PEdge pEdge2 = (PEdge)pElem2;

	if (((pEdge1->nodeA == pEdge2->nodeA) && (pEdge1->nodeB == pEdge2->nodeB)) || ((pEdge1->nodeA == pEdge2->nodeB) && (pEdge1->nodeB == pEdge2->nodeA)) ){
		if (pEdge1->weight == pEdge2->weight)
			return TRUE;
	}
	return FALSE;
}

static COMP_FUNC cmp_ver(PElem pElem1, PElem pElem2) {
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
	PVertex new_vertex = (PVertex)malloc(sizeof(Vertex));

	/*Sanity Check*/
	if ((s == NULL) || (vertex_num < 0))   //check if this check is fine mayber need more
		return FALSE;
	
	new_vertex->serialNumber = vertex_num;

	if (SetAdd(s->Vertex_set, new_vertex) == FALSE) //adding the vertex to the Graph
		return FALSE;

	return TRUE;
}

Bool GraphAddEdge(PGraph s, int vertex1, int vertex2, int weight)
{
	PEdge new_edge = (PEdge)malloc(sizeof(Edge));

	/*Sanity Check*/
	if ((s == NULL) || (vertex1 < 0) || (vertex2 < 0) || (weight < 0))   //check if this check is fine mayber need more
		return FALSE;
   /* here we check if those vertexes are exist*/

	if ((SetFindElement(s->Edge_set, vertex1) == NULL)) //means we don't have it 
		return FALSE;

	if ((SetFindElement(s->Edge_set, vertex2) == NULL)) //means we don't have it 
		return FALSE;

	new_edge->nodeA = vertex1;
	new_edge->nodeB = vertex2;
	new_edge->weight = weight;

	if (SetAdd(s->Edge_set, new_edge) == FALSE) //adding the vertex to the Graph
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
void GraphDestroy(PGraph);


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