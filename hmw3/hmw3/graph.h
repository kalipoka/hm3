/*
 * graph.h
 *
 *  Created on: 22 αιεπ 2016
 *      Author: Shirel_local
 */

#ifndef GRAPH_H_
#define GRAPH_H_

typedef struct _Graph* PGraph;

typedef struct _Vertex
{
	int serialNumber;
} Vertex;

typedef struct _Edge
{
	struct _Vertex* nodeA;
	struct _Vertex* nodeB;
	int weight;
} Edge;

typedef struct _Vertex* PVertex;
typedef struct _Edge* PEdge;

PGraph GraphCreate(); /*DONE*/
void GraphDestroy(PGraph); /*Ofir - DONE need to check if leaks*/
Bool GraphAddVertex(PGraph, int); /*DONE*/
Bool GraphAddEdge(PGraph pGraph, int vertex1, int vertex2, int weight); /*Alex - DONE*/
PSet GraphNeighborVertices(PGraph, int); /*Alex*/
Bool GraphFindShortestPath(PGraph pGraph, int source, int* dist, int* prev); 

int GraphGetNumberOfEdges(PGraph); /*Ofir  -DONE*/ 
int GraphGetNumberOfVertices(PGraph); /*Alex  -DONE*/

PSet GraphVerticesStatus(PGraph); /*Alex -DONE*/
PSet GraphEdgesStatus(PGraph); /*Ofir  -DONE*/

#endif /* GRAPH_H_ */
