#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ST.h"

typedef struct _Item {
    char *nome, *rete;
} Item;

static char *KEYget(Item data) {
    return data.nome;
}

static int KEYcompare(char *h, char *k) {
    return strcmp(h, k);
}

static Item ITEMnew(char *nome, char *rete) {
    Item data;
    data.nome = strdup(nome);
    data.rete = strdup(rete);
    return data;
}

static int ITEMcheckVoid(Item data) {
    return (data.nome==NULL && data.rete==NULL);
}

// inizio st

struct _ST {
    Item *a, z;
    int *indexToHash, *hashToIndex;
    int maxN, size;
    int M; // il numero di caselle della tabella di hash (di solito 2*maxN)
};

ST STinit(int maxN) {
    ST st;
    int i;
    st = malloc(sizeof(*st));
    if (st == NULL) {
        printf("Memory allocation error\n");
        return NULL;
    }
    st->maxN=maxN;
    st->M=2*maxN+1; // almeno è dispari
    st->size=0;
    
    st->a=malloc(st->M*sizeof(Item)); // allocazione doppia per hashing
    for (i=0; i<st->M; i++) st->a[i].nome=NULL, st->a[i].rete=NULL;
    
    st->hashToIndex=malloc(st->M*sizeof(int)); // come sopra
    for (i=0; i<st->M; i++) st->hashToIndex[i]=-1;
    
    st->indexToHash=malloc(st->maxN*sizeof(int)); // quasi come sopra
    for (i=0; i<st->maxN; i++) st->indexToHash[i]=-1;
        
    return st;
}

int STcount(ST st) {
  return st->size;
}

int full (ST st, int i) {
    return !ITEMcheckVoid(st->a[i]);
}

int hash (char *v, int M) {
    int h=0, base=127;
    if (v==NULL) return -1;
    for (; *v; v++) h=(base*h+*v)%M;
    return h;
}

int STinsert(ST st, char *nome, char *rete, int index) {
    // inserisce il nodo che ha indice index nel grafo
    
    int i=hash(nome, st->M);
    while (full(st, i))
         // finché pieno vado a quello dopo (posso anche non entrare nel while)
         i=(i+1)%st->M; // il modulo mi serve se sforo il massimo
    st->a[i]=ITEMnew(nome, rete);
    st->indexToHash[index]=i;
    st->hashToIndex[i]=index;
    st->size++;
    return 1;
}

int STsearch(ST st, char *k) {
    int i;
    if (st->size==0) return -1;
    i=hash(k, st->M);
    // gestione hash con linear probing
    while (full(st, i)) { // fiché c'è qualcosa fai la KEYcompare
        // non ritorna l'indice dell'hash ma l'indice del grafo!!
        if (KEYcompare(k, KEYget(st->a[i]))==0) return st->hashToIndex[i];
        else i++;
    }
    return -1;
}

char *STgetNome(ST st, int i){
    return st->a[st->indexToHash[i]].nome;
}

char *STgetRete(ST st, int i){
    return st->a[st->indexToHash[i]].rete;
}

void STdump(ST st) {
    int i;
    puts("");
    for (i=0; i< st->M; i++)
        printf("%2d (%2d) - %s\n", i, hash(st->a[i].nome, st->M),  st->a[i].nome);
}

int STsameSubnet(ST st, int v, int w) {
    return strcmp(STgetRete(st, v), STgetRete(st, w))==0;
}
