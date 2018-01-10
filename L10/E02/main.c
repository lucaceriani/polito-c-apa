#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BST.h"
#include "Item.h"

#define ID_LEN 3

typedef struct {
    char id[ID_LEN+1];
    int numBici;
    int numPosti;
} Stazione;

int getStationIndexFromId(Stazione*, int, char*);
int leggiTessera(BST, char*, Stazione*, char*, int, int);

int main(int argc, char **argv) {
    
    FILE *fp;
    Stazione *stazioni;
    BST bst=BSTinit();
    
    char stazId[ID_LEN+1], cardId[ID_LEN+1];
    int nStaz, i, tempo, j;
    
    if (argc!=2) puts("Errore nei parametri!"), exit(EXIT_FAILURE);
    if ((fp=fopen(argv[1], "r"))==NULL)
        printf("Impossibile aprire il file \"%s\"!\n", argv[1]), exit(EXIT_FAILURE);
    fscanf(fp, "%d", &nStaz);
    // allocazione del vettore delle stazioni
    stazioni=(Stazione*)malloc(nStaz*sizeof(Stazione));
    for (i=0; i<nStaz; i++) {
        fscanf(fp, "%s %d %d", stazioni[i].id, &stazioni[i].numBici, &stazioni[i].numPosti);
    }
    fclose(fp);
    
    puts("Inserire <stationID> <cardID> <tempo>:");
    while (scanf("%s %s %d", stazId, cardId, &tempo)==3) {
        puts("Inserire <stationID> <cardID> <tempo>:");
        leggiTessera(bst, cardId, stazioni, stazId, tempo, nStaz);
    }    
    
    return 0;
}

int getStationIndexFromId(Stazione *stazioni, int nStaz, char *id) {
    int i;
    for (i=0; i<nStaz; i++)
        if (strcmp(id, stazioni[i].id)==0) return i;

    return -1;
}

int leggiTessera(BST bst, char *cardId, Stazione *stazioni, char *stationId, int time, int nStaz) {
    //BSTinsert_root(bst, ITEMnew(cardId, 0 , 44, nStaz));
    int stazIndex=getStationIndexFromId(stazioni, nStaz, stationId);
    if (stazIndex==-1) {
        puts("Errore! Impossibile trovare la stazione!");
        return -1;
    }
    
    Item user=BSTsearch(bst, cardId);
       
    if (user==NULL){
        BSTinsert_leafR(bst, ITEMnew(cardId, 0, time, nStaz));
    }
    user=BSTsearch(bst, cardId);


    if (ITEMgetNoleggio(user)==1) {
        if (stazioni[stazIndex].numBici<stazioni[stazIndex].numPosti) {
            ITEMsetNoleggio(user, 0);
            stazioni[stazIndex].numBici++; // ficco la bici
            ITEMaggiornaStazione(user, stazIndex);
        }
    } else {
        if (stazioni[stazIndex].numBici>0) {
            ITEMsetNoleggio(user, 1);
            stazioni[stazIndex].numBici--; // ficco la bici
            ITEMaggiornaStazione(user, stazIndex);
        }
    }
    
    return 0;
}