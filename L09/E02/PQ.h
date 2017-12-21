#ifndef PQ_H_INCLUDED
#define PQ_H_INCLUDED

#include "Item.h"

typedef struct _PQ* PQ;


PQ PQnew();
void PQfree(PQ);
void PQinsert(PQ, Item);
void PQdisplay(FILE*, PQ, const char*);
void PQdelete(PQ, int);

Item PQgetItem(PQ, int);
void PQchange (PQ, int, Item);
Item PQextractHead(PQ);
int PQisBattlePossible(PQ);

#endif
