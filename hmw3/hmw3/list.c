/* list.c */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "list.h"


/************************DEFINITIONS**********************/

/* Define one node of the list*/
typedef struct node_t {
	PElem element;           //our element which can be any type
	struct node_t* pNext;     //pointer to the next node on the list towards the END
} NODE, *PNODE;

/* Define the list*/
struct List_ {
	PNODE head;				/* head of the linked list not sure of this element is needed here */
	int size;				/* the number of elements in the list */
	CLONE_FUNC cpy_elem;	/* Clone element function */
	DESTROY_FUNC dstr_elem; /* Destroy element function */
	PNODE iterator;         /* The ITERAOR ***************************should it be here?*/
} List;

/********************************FUNCTIONS*************************************/

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
		PElem tmp = s->cpy_elem(Elem);
		if (!tmp) return FAIL;
		s->head->element=tmp;
		s->head->pNext = NULL; // no next element
	}

	//we have a head already - insert the new element to the head, and keep the previous element
	else
	{ 
		/*Memory Allocc #3                      !!!!!!!!!!!!!!!!!!!!!!!!!!*/
		PNODE new_node = (PNODE)malloc(sizeof(NODE));
		if (!new_node) return FAIL;

		new_node->element = s->cpy_elem(Elem);
		if (new_node->element == NULL)
			return FAIL;
		new_node->pNext = s->head;
		//if ((s->head->element = s->cpy_elem(Elem)) == NULL)
			//return FAIL;
		//s->head->pNext = s->head;        //trying here
		s->head = new_node;
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
	if (s->iterator->pNext == NULL)  // Means we got to the end of the list  - absered in the forum
		return NULL;                

	s->iterator = s->iterator->pNext;  // Move the iterator one next

	temp = s->iterator->element;	// select the element

	return temp;
}

/****************************
*ListRemove function
*The function removes the node that the itertor points for     $$$ need to use delete func and need to free memory alloc
*Argument (PList List)
*Output (Result) - did we succeed in deleteing the swag?
*****************************/
Result ListRemove(PList s)
{
	PNODE temp;

	if (s == NULL)
		return FAIL;
	if (s->iterator == NULL)
		return FAIL;
	
	s->dstr_elem(s->iterator->element);
	

	if (s->iterator == s->head)                      //means we need to change the head
	{
		temp = s->head;
		s->head = s->head->pNext;
		s->size--;

		free(temp);							//free elememt  
		s->iterator = NULL;				
		return SUCCESS;
	}
	
	else
	{
		temp = s->head;
		while (temp->pNext != s->iterator)                //searching for the element before the deleted one
		{
			temp = temp->pNext;
		}
		//here we found the element that is previous to the one that we need to delete
		temp->pNext = s->iterator->pNext;
		free(s->iterator);
		s->size--;
		s->iterator = NULL;
		return SUCCESS;
	}
}

/****************************
*ListDestroy function
*The function deletes the list - need to use user's destroy function for elements,
*and to free malloc of list and of all the nodes
*Argument (PList List)
*Output () - nuber of nodes
*****************************/
void ListDestroy(PList s)
{
	if (s == NULL)
		return;

	PNODE tmp = s->head;

	while (tmp)
	{
		s->head = s->head->pNext;
		s->dstr_elem(tmp->element);
		free(tmp);
		tmp = s->head;
	}	
	
	// here we are done destroyng all the nodes, So we let FREE the List
	free(s);
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

/*********************************** Deeebugy & Testen ****************************************/
/*
int main()
{
	PList trying;
	CLONE_FUNC cpy_elem = 1;
	DESTROY_FUNC dstr_elem = 1;
	int Elem1 = 2;
	char Elem2 = 'c';
	int Elem3 = 5;
	char Elem4[3] = "sne";   

	int godel;
	PElem first_el, next_el_1, next_el_2, next_el_3, next_el_4, next_el_5;
	
	// test ListCreate 
	printf("Hola negritos\n");
	trying = ListCreate(cpy_elem, dstr_elem);
	printf("The list was created\n\n");

	// test ListAdd 
	printf("Add the 1st Element (int type)\n");
	Result res1 = ListAdd(trying, Elem1);
	printf("Adding 1st Element DONE\n\n");
	
	printf("Add the 2nd Element (char type)\n");
	Result res2 = ListAdd(trying, Elem2);
	printf("Adding the 2nd Element (char type) DONE \n\n");

	printf("Add the 3nd Element (string type)\n");
	Result res3 = ListAdd(trying, Elem3);
	printf("Adding the 3d Element (string type) DONE \n\n");

	Result res5 = ListAdd(trying, Elem4);

	// test ListGetSize 
	godel = ListGetSize(trying);

	// test ListGetFirst 
	printf("Recieve the 1st element\n");
	first_el = ListGetFirst(trying);
	printf("Recieve the 1st element DONE\n\n");

	//test ListGetNext 
	
	printf("Recieve the next element &1 \n");
	next_el_1 = ListGetNext(trying);
	printf("Recieve the next element &1 DONE\n\n");
	
	printf("Recieve the next element &2 \n");
	next_el_2= ListGetNext(trying);
	printf("Recieve the next element &2 DONE\n\n");
	
	printf("Recieve the next element &3 \n");
	next_el_3 = ListGetNext(trying);
	printf("Recieve the next element &3 DONE\n\n");

	printf("Recieve the next element &4 Should return Null \n");
	next_el_4 = ListGetNext(trying);
	printf("Recieve the next element &4 DONE\n\n");

	printf("Recieve the next element &4 Should return Null \n");
	next_el_5 = ListGetNext(trying);
	printf("Recieve the next element &4 DONE\n\n");

	
	//test ListRemove #1 - remove head
	printf("Remove the head \n");
	Result res4 = ListRemove(trying);
	printf("Remove the head is DONE\n\n");
	
	//test ListDestroy
	printf("Destroy The List \n");
	ListDestroy(trying);
	printf("Destroy The List DONE \n\n");
	

	Result res5 = ListAdd(trying, Elem4);
	//here we delete the 3rd elemnts out of 4
	trying->iterator = trying->head->pNext->pNext;

	//test ListRemove #2 - remove element from the middle
	printf("Remove the middle element \n");
	Result res6 = ListRemove(trying);
	printf("Remove the middle element is DONE\n\n");
	
	return 0;
}
*/