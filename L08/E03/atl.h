#ifndef ATL_H_INCLUDED
#define ATL_H_INCLUDED

#include "liste.h"

#define MAX_ESERCIZIO 50
#define MAX_NOME 25
#define LUNG_CODICE 5

typedef struct {
    char nome[MAX_NOME+1];
    char cognome[MAX_NOME+1];
    char cognomenome[2*MAX_NOME+1];
    char categoria[MAX_NOME+1];
    char codice[LUNG_CODICE+1];
    char data[11];
    int ore;
    Lista *listaEs;
} Atleta;

Atleta *getAtletaFromNode(link x);
link cercaAtleta(link, char*);

void stampaAtleta(link, FILE*);
int getNAtleti(Lista*);

#endif
