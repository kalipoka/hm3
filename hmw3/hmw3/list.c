/*
* list.c
*
*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "list.h"
#include "set.h"
#include "graph.h"

/*******DEFINITIONS*********/

/* Define one node of the list*/

typedef struct node_t {
	PElem element;           //our element which can be any type
	struct node_t* pNext;     //pointer to the next elemnet on the list
} NODE, *PNODE;

/* Define the list*/
typedef struct List_ {
	PNODE head;				/* head of the linked list */
	int size;				/* the number of elements in the list */
	CLONE_FUNC cpy_elem;	/* Clone elemnt function */
	DESTROY_FUNC dstr_elem; /* Destroy elemnt function */
	PNODE iterator;         /* The ITERAOR ***************************should be here?*/
} List, *PList;

/* Create the list*/
PList ListCreate(CLONE_FUNC cpy_elem, DESTROY_FUNC dstr_elem)
{
	PList s;
	/*Sanity Check*/
	if ((cpy_elem == NULL) || (dstr_elem == NULL))
		return NULL;
	/*Memory Allocc #1                      !!!!!!!!!!!!!!!!!!!!!!!!!!*/
	s = (PList)malloc(sizeof(List));
	if (s == NULL)
		return NULL;
	/*Memory Allocc #2                     !!!!!!!!!!!!!!!!!!!!!!!!!!*/
	s->head = (PNODE)malloc(sizeof(NODE));
	if (s->head == NULL) {
		free(s);
		return NULL;
	}

	s->cpy_elem = cpy_elem;
	s->dstr_elem = dstr_elem;
	s->size = 0;
	s->iterator = NULL;                    /*Iterator - to chech what should be his inital value*/
	return s;
}
/*
*ListGetSize function
*Argument (PList List)
*Output (int) - nuber of nodes
*/
int ListGetSize(PList List)
{
	if (List == NULL)
		return NULL;
	return List->size;
}

/*
*Destroy function
*Argument (PList s)
*Output ()
*/
/*
void ListDestroy(PList s)
{
	if (s == NULL)
		return;
}
*/
int main()
{
	CLONE_FUNC cpy_elem = 1;
	DESTROY_FUNC dstr_elem = 1;
	PList trying;
	printf("Hola negritos\n");
	trying = ListCreate(cpy_elem, dstr_elem);
	printf("The list was created\n");
	return 0;
}