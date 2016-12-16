/*
 * list.h
 *
 *  Created on: 22 αιεπ 2016
 *      Author: Shirel_local
 */


typedef struct List_* PList;
typedef void* PElem;                  //can be any type
typedef enum{FAIL, SUCCESS} Result;

/*User functions*/
typedef PElem (*CLONE_FUNC)(PElem);
typedef void (*DESTROY_FUNC)(PElem);

/*Interface functions*/
PList ListCreate(CLONE_FUNC, DESTROY_FUNC);    //done compiled and checked
void ListDestroy(PList);
Result ListAdd(PList, PElem);                   //done compiled and checked
PElem ListGetFirst(PList);						//done compiled and checked
PElem ListGetNext(PList);						//done compiled and checked

Result ListRemove(PList);
int ListGetSize(PList);                        //done compiled and checked

