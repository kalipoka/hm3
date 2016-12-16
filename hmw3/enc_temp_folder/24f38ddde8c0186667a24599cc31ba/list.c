/* list.c */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "list.h"
#include "set.h"
#include "graph.h"

/************************DEFINITIONS**********************/

/* Define one node of the list*/
typedef struct node_t {
	PElem element;           //our element which can be any type
	struct node_t* pNext;     //pointer to the next node on the list
} NODE, *PNODE;

/* Define the list*/
struct List_; 
typedef struct List_ {
	PNODE head;				/* head of the linked list not sure of this element is needed here */
	int size;				/* the number of elements in the list */
	CLONE_FUNC cpy_elem;	/* Clone elemnt function */
	DESTROY_FUNC dstr_elem; /* Destroy elemnt function */
	PNODE iterator;         /* The ITERAOR ***************************should it be here?*/
} List, *PList;

/*******************FUCKTIONS***********************/

/*****************************
*ListCreate function
*The function creates the list, allocates memory for the List and for the Head
*Argument (CLONE_FUNC cpy_elem, DESTROY_FUNC dstr_elem)
*Output (PList s)
*****************************/
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
	/* nulify the head*/
	s->head->element = NULL;  // no elements yet;
	s->head->pNext = NULL; // no next nodes
	s->cpy_elem = cpy_elem;
	s->dstr_elem = dstr_elem;
	s->size = 0;
	s->iterator = NULL;                    /*Iterator - to chech what should be his inital value*/
	return s;
}

/*****************************
*LisAdd function
*Argument (PList List, PElem Elem)
*Output (Result)
*****************************/

Result ListAdd(PList s, PElem Elem)
{
	if ((s == NULL) || (Elem == NULL))
		return FAIL;
	//check if the list is empy - has 0 nodes - we need to create the head - We did MALLOC for the head already
	if (s->size == 0)
	{
		///  if ((s->head->element = cpy_elem(Elem)) == NULL)          //Activate after debug
		s->head->element = Elem;									//Delete after debug
		//	return FAIL;											//Activate after debug
		s->head->pNext = NULL; // no next elemnts yet
	}

	//we have a head already - insert the new element to the head, and keep the previous element
	else
	{ 
		/*Memory Allocc #3                      !!!!!!!!!!!!!!!!!!!!!!!!!!*/
		PNODE new_node = (PNODE)malloc(sizeof(NODE));
		if (!new_node) return FAIL;

		new_node->element = Elem;
		new_node->pNext = s->head;
		//if ((s->head->element = cpy_elem(Elem)) == NULL)   //Activate after debug
		//	return FAIL;                                     //Activate after debug
		s->head = new_node;							//Delete after debug
	}
	s->size++;
	s->iterator = NULL; // illegal as stated in function description
	return SUCCESS;
}

/*****************************
*ListGetFirst function
*function returns the 1st element and aslo put the iterator to the head of the list
*Argument (PList s)
*Output 1st element (PElem elem)
*****************************/
PElem ListGetFirst(PList s)
{
	PElem temp;

	if (s == NULL)
		return NULL;

	if (s->size == 0)   //means we do not have a first element cause list is empty
		return NULL;

	temp = s->head->element; // this is the 1st element
	s->iterator = s->head;  //foloowing question instruction iterator needs to point to head
	return temp;
	
}

/*****************************
*ListGetNext function
*function returns the the element That the iterator points to, and moves the Itertor one step
*to the end
*Argument (PList s)
*Output iterator pointed element (PElem elem)
*****************************/
PElem ListGetNext(PList s)
{
	PElem temp;

	if (s == NULL)
		return NULL;
	if (s->iterator == NULL)
		return NULL;
	if (s->iterator->pNext == NULL)  // Means we got to the end of the list
		return NULL;

	temp = s->iterator->element;
	s->iterator = s->iterator->pNext;  // Move the iterator one next

	return temp;
}

/****************************
*ListGetSize function
*Argument (PList List)
*Output (int) - nuber of nodes
*****************************/
int ListGetSize(PList List)
{
	if (List == NULL)
		return 0;
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
	/********************** Deeebugy & Testen ***************************/
	PList trying;
	CLONE_FUNC cpy_elem = 1;
	DESTROY_FUNC dstr_elem = 1;
	int Elem1 = 2;
	char Elem2 = 'c';
	int Elem3 = 5;
	int godel;
	PElem first_el, next_el_1, next_el_2, next_el_3;
	
	/* test ListCreate */
	printf("Hola negritos\n");
	trying = ListCreate(cpy_elem, dstr_elem);
	printf("The list was created\n");

	/* test ListAdd */
	printf("Add the 1st Element (int type)\n");
	Result res1 = ListAdd(trying, Elem1);
	printf("Adding 1st Element DONE\n");
	
	printf("Add the 2nd Element (char type)\n");
	Result res2 = ListAdd(trying, Elem2);
	printf("Adding the 2nd Element (char type) DONE \n");

	printf("Add the 3nd Element (string type)\n");
	Result res3 = ListAdd(trying, Elem3);
	printf("Adding the 3d Element (string type) DONE \n");

	/* test ListGetSize */
	godel = ListGetSize(trying);

	/* test ListGetFirst */
	printf("Recieve the 1st element\n");
	first_el = ListGetFirst(trying);
	printf("Recieve the 1st element DONE\n");

	/* test ListGetNext */
	printf("Recieve the next element &1 \n");
	next_el_1 = ListGetNext(trying);
	printf("Recieve the next element &1 DONE\n");

	printf("Recieve the next element &2 \n");
	next_el_2= ListGetNext(trying);
	printf("Recieve the next element &2 DONE\n");

	printf("Recieve the next element &3 \n");
	next_el_3 = ListGetNext(trying);
	printf("Recieve the next element &3 DONE\n");

	return 0;
}