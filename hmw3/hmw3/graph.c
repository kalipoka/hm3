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
	PVertex Vertex_head;   //not sure that enough parameters in the struct
	PEdge Edge_head;	  //not sure that enough parameters in the struct

} Graph;

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

	s->Edge_head = SetCreate(cmpFunc, cloneFunc, destroyFunc);    //creates the 1st Edge (keshet)
	if (s->Edge_head==NULL)
		return NULL;

	s->Edge_head->nodeA = NULL;
	s->Edge_head->nodeB = NULL;
	s->Edge_head->weight = 0;

	s->Vertex_head = SetCreate(cmpFunc, cloneFunc, destroyFunc);   //creates the 1st Vertex (tsomet)
	if (s->Vertex_head == NULL)
		return NULL;

	s->Vertex_head->serialNumber = 0;

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
	if ((s == NULL) || (vertex_num < 0))   //check if this check is fine
		return FALSE;
	
	if(s->Vertex_head->serialNumber!=vertex_num)  //covers 2 check: 1. if the vertex exists  2. if the number is legit
		return FALSE;

	if (SetAdd(s->Vertex_head, vertex_num) == FALSE) //adding the vertex to the Graph
		return FALSE;

	s->Vertex_head->serialNumber++;

	return TRUE;
}
/*********************************** Deeebugy & Testen ****************************************/
int main()
{
	PGraph tryingGraph;
	printf("Hola negritos Creating Graph\n");
	tryingGraph = GraphCreate();
	printf("The Graph was created DONE \n\n");

	printf("Adding Vertex Check\n");
	Bool res1 = GraphAddVertex(tryingGraph, 0); // waiting for response from forum about adding zero Vertex
	printf("The Vertex Was Added \n\n");

	return 0;
	
}