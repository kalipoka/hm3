/* graph.c */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "list.h"
#include "set.h"
#include "graph.h"

/************************DEFINITIONS**********************/

struct _Graph{
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
	int i=0;
	while (elem)
	{
		PEdge ver = (PEdge)elem;
		printf("{%d, %d}\n", ver->nodeA->serialNumber, ver->nodeB->serialNumber);
		elem = SetGetNext(s->Edge_set);
	}
}

static int count_verteces(PGraph s)
{
	PElem elem = SetGetFirst(s->Vertex_set);
	int i = 0;
	while (elem)
	{
		elem = SetGetNext(s->Vertex_set);
		i++;
	}
	return i;
}

static int count_edges(PGraph s)
{
	PElem elem = SetGetFirst(s->Edge_set);;
	int i = 0;
	while (elem)
	{
		elem = SetGetNext(s->Edge_set);
		i++;
	}
	return i;
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

static PElem find_edge_by_nodes(PGraph s, int nodeA, int nodeB)
{
	PElem elem = SetGetFirst(s->Edge_set);

	while (elem)
	{
		PEdge pEdge = (PEdge)elem;
		PVertex ed1nodA = (PVertex)pEdge->nodeA;
		PVertex ed1nodB = (PVertex)pEdge->nodeB;
		if (((ed1nodA->serialNumber == nodeA) && (ed1nodB->serialNumber == nodeB)) || ((ed1nodA->serialNumber == nodeB) && (ed1nodB->serialNumber == nodeA)))
			return elem;
		elem = SetGetNext(s->Edge_set);
	}
	return NULL;
}

static PElem find_edge(PGraph s, PElem pElem)
{
	return SetFindElement(s->Edge_set, pElem);
}

static void destroy_ver(PElem pElem) ////////////////////////////////////////////////////////////////////////////////////////
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

	PVertex ed1nodA = (PVertex)pEdge1->nodeA;
	PVertex ed1nodB = (PVertex)pEdge1->nodeB;
	PVertex ed2nodA = (PVertex)pEdge2->nodeA;
	PVertex ed2nodB = (PVertex)pEdge2->nodeB;


	if (((ed1nodA->serialNumber == ed2nodA->serialNumber) && (ed1nodB->serialNumber == ed2nodB->serialNumber)) || ((ed1nodA->serialNumber == ed2nodB->serialNumber) && (ed1nodB->serialNumber == ed2nodA->serialNumber)))
	return TRUE;

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

static PElem find_similar_edge(PGraph s, PElem pElem)
{
	PElem elem_index = SetGetFirst(s->Edge_set);

	while (elem_index)
	{
		Bool bool = cmp_edg(pElem, elem_index);
		if (bool == TRUE)
			return elem_index;
		elem_index = SetGetNext(s->Edge_set);
	}
	return NULL;
}

Bool GraphAddVertex_for_clone(PGraph s, int vertex_num) //with the check if vertex is added by order
{
	/*Sanity Check*/
	if ((s == NULL) || (vertex_num < 0))   //check if this check is fine mayber need more
		return FALSE;

	if (find_vertex_by_num(s, vertex_num)) //if we've found a similar vertex
		return FALSE;

	PVertex new_vertex = (PVertex)malloc(sizeof(Vertex));
	if (!new_vertex) return FALSE;
	new_vertex->serialNumber = vertex_num;

	if (SetAdd(s->Vertex_set, new_vertex) == FALSE)//adding the vertex to the Graph
	{
		free(new_vertex);
		return FALSE;
	}
	free(new_vertex);
	return TRUE;
}


static Bool Clone_graph(PGraph o, PGraph t)
{
	PElem elem = SetGetFirst(o->Vertex_set);
	PElem elemcopy = NULL;
	while (elem)
	{
		PVertex ver = (PVertex)elem;
		if (GraphAddVertex_for_clone(t, ver->serialNumber) == FALSE)  return FALSE;
		elem = SetGetNext(o->Vertex_set);
	} //we want the source to be the first element in our Q

	elem = SetGetFirst(o->Edge_set);
	while (elem)
	{
		PEdge edg = (PEdge)elem;
		if (GraphAddEdge(t,((PVertex)edg->nodeA)->serialNumber, ((PVertex)edg->nodeB)->serialNumber, edg->weight) == FALSE) return FALSE;
		elem = SetGetNext(o->Edge_set);
	}
	return TRUE;
}


destroyAllEdges(PGraph s)
	{
		PElem elem = SetGetFirst(s->Edge_set);
		while (elem)
		{
			PElem tmp = elem;
			elem = SetGetNext(s->Edge_set);
			PEdge edg = (PEdge)tmp;
			destroy_ver(edg->nodeA);
			destroy_ver(edg->nodeB);
		}
		SetDestroy(s->Edge_set);
	}

destroyAllVerteces(PGraph s)
{
	SetDestroy(s->Vertex_set);
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

	if (count_verteces(s) != vertex_num)
		return FALSE;

	PVertex new_vertex = (PVertex)malloc(sizeof(Vertex));
	if (!new_vertex) return FALSE;
	new_vertex->serialNumber = vertex_num;

	if (SetAdd(s->Vertex_set, new_vertex) == FALSE)//adding the vertex to the Graph
	{
		free(new_vertex);
		return FALSE;
	}
	free(new_vertex);
	return TRUE;
}

/*****************************
*GraphAddEdge function
*The function add an Edge to the graph
*Argument (PGrapf s, int vertex1, int vertex2, int weight)
*Output (TRUE/FALSE)
*****************************/

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

	if (find_similar_edge(s, new_edge)) //if we've found a similar edge
	{
		free(new_edge);
		return FALSE;
	}
	// cloning the verteces for the new edge
	PElem elem = NULL;
	new_edge->nodeA = clone_ver(new_edge->nodeA);
	new_edge->nodeB = clone_ver(new_edge->nodeB);
	new_edge->weight = weight;

	if (SetAdd(s->Edge_set, new_edge) == FALSE)//adding the vertex to the Graph
	{
		free(new_edge);
		return FALSE;
	}

	free(new_edge);
	return TRUE;
}

/*****************************
*GraphNeighborVertices function
*The creates a set of th neighbours
*Argument (PGrapf s, int vertex_num)
*Output Pset or NULL
*****************************/
PSet GraphNeighborVertices(PGraph s, int vertex_num)
{
	if ((s == NULL) || (vertex_num < 0)) return NULL;

	PVertex the_vertex = (PVertex)find_vertex_by_num(s, vertex_num);   // here we check if we have this verete at all

	if (the_vertex == NULL) return NULL;    //means no such vertex here

	PSet neigbours;                         // create the set of the neighbour vertex

	if (NULL == (neigbours = SetCreate(cmp_ver, clone_ver, destroy_ver)))
		return NULL;

	PElem elem = SetGetFirst(s->Edge_set);
	while (elem)
	{
		PEdge edg = (PEdge)elem;
		if ((edg->nodeA->serialNumber == vertex_num) || (edg->nodeB->serialNumber == vertex_num)) // means there is a neighbour
		{

			PVertex new_vertex = (PVertex)malloc(sizeof(Vertex));                                // #$%#%#$%#%#$ MALLOC
			if (!new_vertex) return NULL;

			if (edg->nodeA->serialNumber != vertex_num)                                //here we check which one of the Kzavot we need to save
				new_vertex->serialNumber = edg->nodeA->serialNumber;
			else                                                                      // Node B
				new_vertex->serialNumber = edg->nodeB->serialNumber;
			
			if (SetAdd(neigbours, new_vertex) == FALSE)//adding the vertex to the Graph
				{
					free(new_vertex);
					return NULL;
				}
			free(new_vertex);
		}
		elem = SetGetNext(s->Edge_set);
	}
	return neigbours;

}


Bool GraphFindShortestPath(PGraph pGraph, int source, int* dist, int* prev)
{
	PElem source_elem = find_vertex_by_num(pGraph, source);
	if (!source_elem) return FALSE; //check if source is legal
	// create vertex set Q - we already have dont we

	PGraph tmp_graph = GraphCreate();

	// cloning original graph to tmp
	if (Clone_graph(pGraph, tmp_graph) == FALSE) return FALSE;

	int i = 0, v = 0;
	for ( i = 0, v = GraphGetNumberOfVertices(tmp_graph); i < v; i++) { // Initialization
		dist[i] = count_edges(tmp_graph)*10; // Unknown distance from source to v
		prev[i] = -1; // Previous node in optimal path from source
	}
	
	dist[source] = 0; // Distance from source to source
	prev[source] = source;

	PElem elem = SetGetFirst(tmp_graph->Vertex_set);

	while (elem) {
		//find minimum in temp_graph
		PElem min_vertex_elem = elem, tmp = NULL;
		PElem min_next_elem = SetGetNext(tmp_graph->Vertex_set);
		while (min_vertex_elem && min_next_elem)
		{
			PVertex ver1 = (PVertex)min_next_elem;
			PVertex ver2 = (PVertex)min_next_elem;
			if (dist[ver1->serialNumber] > dist[ver2->serialNumber])
				min_vertex_elem = min_next_elem;
			min_next_elem = SetGetNext(tmp_graph->Vertex_set);
		}

		PVertex ver_min = (PVertex)min_vertex_elem; //this vertex has the minimum distance
			
		// check neibours
		PSet neibour_set = GraphNeighborVertices(pGraph, ver_min->serialNumber);
		PElem neibour = SetGetFirst(neibour_set);
		int alt = -1;
		while (neibour)
		{
			PVertex neibour_vex = (PVertex)neibour;
			PEdge common_edge = find_edge_by_nodes(tmp_graph, ver_min->serialNumber, neibour_vex->serialNumber);
			alt = dist[ver_min->serialNumber] + common_edge->weight;
			if (alt < dist[neibour_vex->serialNumber])
			{
				dist[neibour_vex->serialNumber] = alt;
				prev[neibour_vex->serialNumber] = ver_min->serialNumber;
			}
			neibour = SetGetNext(neibour_set);
		}
		SetDestroy(neibour_set);
		SetRemoveElement(tmp_graph->Vertex_set, elem);
		elem = SetGetFirst(tmp_graph->Vertex_set);
		//printf("number of verts %d\n", count_verteces(tmp_graph));
	}
	GraphDestroy(tmp_graph);
	return TRUE;
}

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
	destroyAllVerteces(s);
	destroyAllEdges(s);
	free(s);
}
//

/*********************************** Deeebugy & Testen ****************************************/
/*
int main()
{
	PGraph tryingGraph;
	tryingGraph = GraphCreate();

	if (GraphAddVertex(tryingGraph, 0) == FALSE)
		printf("Adding Vertex Check &0 FAILED \n\n");
	
	if (GraphAddVertex(tryingGraph, 1) == FALSE)
		printf("Adding Vertex Check &1 FAILED \n\n");
	
	if (GraphAddVertex(tryingGraph, 2) == FALSE)
		printf("Adding Vertex Check &2 FAILED \n\n");

	if (GraphAddVertex(tryingGraph, 3) == FALSE)
		printf("Adding Vertex Check &2 FAILED \n\n");

	if (GraphAddVertex(tryingGraph, 4) == FALSE)
		printf("Adding Vertex Check &2 FAILED \n\n");

	if (GraphAddVertex(tryingGraph, 5) == FALSE)
		printf("Adding Vertex Check &2 FAILED \n\n");

	
	print_vertex_list(tryingGraph);

	if (GraphAddEdge(tryingGraph, 0, 1, 3) == FALSE)
		printf("Adding Edge Check &0 FAILED \n\n");
	
	if (GraphAddEdge(tryingGraph, 1, 2, 3) == FALSE)
		printf("Adding Edge Check &1 FAILED - cause you are fucking nigha \n\n");

	if (GraphAddEdge(tryingGraph, 0, 3, 3) == FALSE)
		printf("Adding Edge Check &2 FAILED \n\n");

	if (GraphAddEdge(tryingGraph, 3, 4, 3) == FALSE)
		printf("Adding Edge Check &3 FAILED \n\n");

	if (GraphAddEdge(tryingGraph, 4, 5, 3) == FALSE)
		printf("Adding Edge Check &4 FAILED \n\n");
	

	print_edges_list(tryingGraph);
    
	printf("number of vertex %d \n\n", GraphGetNumberOfVertices(tryingGraph));

	printf("number of Edges %d \n\n", GraphGetNumberOfEdges(tryingGraph));
	
	int *d = malloc(GraphGetNumberOfVertices(tryingGraph) * sizeof(int));
	int *p = malloc(GraphGetNumberOfVertices(tryingGraph) * sizeof(int));


	GraphFindShortestPath(tryingGraph, 0, d, p); //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		
	
	for (int i = 0, v = GraphGetNumberOfVertices(tryingGraph); i <v; i++) { // Initialization
		printf("%d %d %d\n", i, d[i], p[i]);
	}



	GraphDestroy(tryingGraph);
	free(p);
	free(d);

	return 0;
}
*/