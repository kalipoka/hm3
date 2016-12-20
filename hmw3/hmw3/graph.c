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

typedef struct _Vertex_Set
{
	PEdge Edge;
	/* Function Pointers */
	COMP_FUNC edge_cmp;
	CLONE_FUNC edge_clone;
	DESTROY_FUNC edge_destroy;
} Edge_Set, PEdge_Set;




CLONE_FUNC cloneFunc = 1;                    // $#%#$%#$%#$%#$#%#$% DELETE BEFORE SUBMIT - for COMPILE ONLY $#%#$%#$%#$%#$#%#$%/
DESTROY_FUNC  destroyFunc = 1;				 // $#%#$%#$%#$%#$#%#$% DELETE BEFORE SUBMIT - for COMPILE ONLY $#%#$%#$%#$%#$#%#$%/
COMP_FUNC cmpFunc = 1;						 // $#%#$%#$%#$%#$#%#$% DELETE BEFORE SUBMIT - for COMPILE ONLY $#%#$%#$%#$%#$#%#$%/
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

	s->Edge_set = SetCreate(cmpFunc, cloneFunc, destroyFunc);    //creates the 1st Edge (keshet)
	if (s->Edge_set==NULL)
		return NULL;

	s->Vertex_set = SetCreate(cmpFunc, cloneFunc, destroyFunc);   //creates the 1st Vertex (tsomet)
	if (s->Vertex_set == NULL)
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
	
	//if(s->Vertex_set->serialNumber!=vertex_num)  //covers 2 check: 1. if the vertex exists  2. if the number is legit
		//return FALSE;

	new_vertex->serialNumber = vertex_num;

	if (SetAdd(s->Vertex_set, new_vertex) == FALSE) //adding the vertex to the Graph
		return FALSE;

	return TRUE;
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
	printf("The Vertex &0 Was Added \n\n");

	printf("Adding Vertex Check &1 \n");
	Bool res2 = GraphAddVertex(tryingGraph, 1); // waiting for response from forum about adding zero Vertex
	printf("The Vertex &1 Was Added \n\n");

	return 0;
	
}