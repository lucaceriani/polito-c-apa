#include <stdio.h>
#include <stdlib.h>

#include "PQ.h"

#define MAX_HEAP_SIZE 1000

#define LEFT(i) ((2*(i))+1)
#define RIGHT(i) ((2*(i))+2)
#define PARENT(i) (((i)-1)/2)

// soluzione adottata: heap min

struct _PQ {
    Item *A;
    int heapsize;
};

PQ PQnew() {
    PQ pq=malloc(sizeof(struct _PQ));
    pq->A=malloc(MAX_HEAP_SIZE*(sizeof *(pq->A)));
    pq->heapsize=0;
    return pq;
}

void PQfree(PQ pq) {
    free(pq->A);
    free(pq);
}

void PQinsert (PQ pq, Item item) {
    int i;
    i = pq->heapsize++;
    while( (i>=1) && (ITEMgreater(pq->A[PARENT(i)], item)) ) {
        pq->A[i] = pq->A[PARENT(i)];
        i = (i-1)/2;
    }
    pq->A[i] = item;
return;
}

void Swap(PQ pq, int i, int j) {
    Item tmp=pq->A[i];
    pq->A[i]=pq->A[j];
    pq->A[j]=tmp;
}



static int exists(PQ pq, int i) {
    return  (i < pq->heapsize) && (i>=0);
}

/*
static int isLeaf(PQ pq, int i) {
    if (!exists(pq,LEFT(i)) && !exists(pq,RIGHT(i))) return 1;
    return 0;
}
 */

void PQdisplay(FILE* fp, PQ pq, const char *mode) {
    int i=0;    
    if (pq->heapsize==0) {
        puts("Vuoto!");
        return;
    }
    
    for (i=0; i<pq->heapsize; i++){
        if (mode[0]=='o') fprintf(fp, "%3d.", i+1);
        ITEMdisplay(fp, pq->A[i], mode);
    }
}

static void HEAPify(PQ pq, int i) {
    int l, r, smallest;
    l = LEFT(i);
    r = RIGHT(i);
    
    // determinazione del minore
    if (exists(pq,l) && (ITEMless(pq->A[l], pq->A[i])))
        smallest = l;
    else
        smallest = i;
    
    if (exists(pq,r) && (ITEMless(pq->A[r], pq->A[smallest])))
        smallest = r;

    // eventuale swap e ricorsione
    if (smallest != i) {
        Swap(pq, i, smallest);
        HEAPify(pq, smallest);
    }
}

void HEAPbuild(PQ pq) {
    int i;
    for (i=(pq->heapsize)/2-1; i>=0; i--) HEAPify(pq, i);
}

void PQdelete(PQ pq, int i) {
    // voglio cancellare l'elemento nella posizione i-esima
    // lo scambio con lultima posizione, faccio risalire il valore fino a dove
    // deve essere e poi applico HEAPify
    
    Swap(pq, i, pq->heapsize-1); // swappo all'ultima posizione
    ITEMfree(pq->A[pq->heapsize-1]); // libero la memoria dell'item
    pq->heapsize--; // riduco la dimensione dell'heap;
    
    HEAPify(pq, i);
}

Item PQgetItem(PQ pq, int i) {
    if (!exists(pq,i)) return NULL;
    return pq->A[i];
}


void PQchange (PQ pq, int i, Item item) {
    Item oldItem=pq->A[i]; // me lo salvo per la free
    while( (i>=1) && (ITEMgreater(pq->A[PARENT(i)], item)) ) {
        pq->A[i] = pq->A[PARENT(i)];
        i = (i-1)/2;
    }
    pq->A[i] = item;
    HEAPify(pq, i);
    
    ITEMfree(oldItem);
    return;
}

int PQisBattlePossible(PQ pq) {
    return (pq->heapsize)>1;
}

Item PQextractHead(PQ pq) {
    if (!exists(pq, 0)) return NULL; // controllo se posso procedere
    
    Item i;
    Swap(pq, 0, pq->heapsize-1);
    i = pq->A[pq->heapsize-1];
    
    pq->heapsize--;
    HEAPify(pq, 0);
    return i;
}
