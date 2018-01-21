#include <stdio.h>
#include <stdlib.h>

#define OK printf("OK\n")

#include "Graph.h"

int main(int argc, char **argv) {
    
    Graph G;
    int nVer, x;
    FILE *fp;
    
    // controllo sui parametri da linea di comando
    if (argc!=2) puts("Errore! Parametri non corretti.\n"), exit(1);
    
    
    printf("Lettura file \"%s\"... ", argv[1]);
    if ((fp=fopen(argv[1], "r"))==NULL)
        printf("Errore! Impossibile aprire il file \"%s\".\n", argv[1]), exit(1);
    OK;
    fscanf(fp, "%d", &nVer);
    printf("Trovato grafo con %d vertici...\n", nVer);
    printf("Inizializzazione grafo... ");
    G=GRAPHinit(nVer);
    if (G==NULL) puts("Errore! Impossibile allocare il grafo!");
    OK;
    printf("Lettura vertici dal file...");
    GRAPHreadVertexFromFile(G, fp);
    OK;
    printf("Generazione delle liste di adiacenza da file... ");
    GRAPHreadAdjList(G, fp);
    OK;
    printf("\nPreme INVIO per continuare..."), getc(stdin);
    
    while (80085) {
        system("cls");
        puts("1. Stampa tutti i vertici");
        puts("2. Stampa numero archi e vertici connessi a un nodo");
        puts("3. Flussi intra/inter-rete totali");
        puts("4. Genera matrice adiacenze");
        printf("> ");
        scanf("%d", &x);
        switch (x) {
        case 1: // stampa tutti i vertici
            GRAPHdumpAllNodes(G);
            break;
        case 2: // numero archi e vertici connessi
            GRAPHdumpNode(G);
            break;
        case 3: // flussi intra/inter rete
            GRAPHfluxes(G);
            break;
        case 4: // matrice adiacenze
            GRAPHgenerateAdjM(G, 1);
            break;
        default:
            exit(0);
        }
        puts("\nPremere INVIO per continuare...");
        getc(stdin);
        getc(stdin);
    }
    return 0;
}