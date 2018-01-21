#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 30

#include "Graph.h"

typedef struct node {
    int v, wt;
    struct node *next;
} *link;

struct _Graph {
    int V, E;
    link *adjL, z;
    int **adjM; // matrice delle adiacenze
    ST st;
};


static link NEW(int v, int wt, link next) { // inteso come nuovo elemento della adjL
    link x = malloc(sizeof *x);
    x->v = v;
    x->wt=wt;
    x->next = next;
    return x;
}


Graph GRAPHinit(int V) {
    int v;
    Graph G = malloc(sizeof *G);
    if (G == NULL) return NULL;

    G->V = V;
    G->E = 0;

    // inizializzazione lista adiacenze
    G->adjL = calloc(G->V,sizeof(link)); // calloc rende NULL i puntatori

    // inizializzazione matrice adiacenze
    G->adjM=(int**)malloc(G->V*sizeof(int*));
    for (v=0; v < G->V; v++)
        G->adjM[v]=(int*)calloc(G->V,sizeof(int));

    G->st = STinit(V);
    if (G->st==NULL) return NULL;

    return G;
}

static void insertE(Graph G, int v, int w, int wt) {
    // dato che è un grafo non orientanto una connessione v->w
    // è anche una connessione w->v, quindi la inserisco
    // inserzione in testa (la più semplice)
    G->adjL[w] = NEW(v, wt, G->adjL[w]);
    G->adjL[v] = NEW(w, wt, G->adjL[v]);
}

void GRAPHreadVertexFromFile(Graph G, FILE *fp) {
    int i;
    char nome[MAX_LEN+1], rete[MAX_LEN+1];
    for (i=0; i< G->V; i++) {
        fscanf(fp, "%s %s", nome, rete);
        STinsert(G->st, nome, rete, i);
    }
}

void GRAPHreadAdjList(Graph G, FILE *fp) {
    char nome1[MAX_LEN+1], nome2[MAX_LEN+1];
    int v, w, wt;
    while (fscanf(fp, "%s %s %d", nome1, nome2, &wt)==3) {
        v=STsearch(G->st, nome1);
        w=STsearch(G->st, nome2);
        insertE(G, v, w, wt);
    }
}

void GRAPHdumpAllNodes(Graph G) {
    int i;
    for (i=0; i<G->V; i++) {
        printf("%2d - %9s, %9s\n", i+1, STgetNome(G->st, i), STgetRete(G->st, i));
    }
}

void GRAPHdumpNode(Graph G) {
    char nome[MAX_LEN+1];
    int i,j;
    link h;
    printf("Nome del nodo: ");
    scanf("%s", nome);
    
    i=STsearch(G->st, nome);
    if (i==-1) { // se non ho trovato niente
        puts("Attenzione! Nome del nodo errato.");
        return;
    }
    for (j=0, h=G->adjL[i]; h!=NULL; h=h->next) {
        printf("%s, %s\n", STgetNome(G->st, h->v), STgetRete(G->st, h->v));
        j++;
    }
    printf("Totale nodi collegati a \"%s\": %d\n", nome, j);
}

void GRAPHgenerateAdjM(Graph G, int print) {
    int i,j;
    link h;
    for (i=0; i<G->V; i++)
        for (h=G->adjL[i]; h!=NULL; h=h->next)
            G->adjM[i][h->v]=h->wt;
    
    if (!print) return;
    
    for (i=0; i<G->V; i++) {
        for (j=0; j<G->V; j++)
            printf("%3d ", G->adjM[i][j]);
        printf("\n");
    }
}

void GRAPHfluxes(Graph G) {
    int i, intra=0, inter=0;
    link h;
    for (i=0; i<G->V; i++)
        for (h=G->adjL[i]; h!=NULL; h=h->next)
            if (h->v>i){ // insomma se non ho già visitato quel nodo
                if (STsameSubnet(G->st, i, h->v))
                    intra+=h->wt;
                else
                    inter+=h->wt;
            }
    printf("Flusso intra-rete: %d\n", intra);
    printf("Flusso inter-rete: %d\n", inter);
}
