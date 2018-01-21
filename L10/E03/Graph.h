#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include "ST.h"

typedef struct _Graph *Graph;

Graph GRAPHinit(int);
void GRAPHreadVertexFromFile(Graph, FILE*);
void GRAPHreadAdjList(Graph, FILE*);
void GRAPHdumpAllNodes(Graph);
void GRAPHdumpNode(Graph);
void GRAPHgenerateAdjM(Graph, int);
void GRAPHfluxes(Graph);


#endif