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

// funzioni di visualizzazione
void stampaPerCategoria(Lista*);
void stampaAnagrafica(Lista*, FILE*);

Atleta *getAtletaFromNode(link x);
Atleta *cercaAtleta(Lista*, char*);


void stampaAtleta(Atleta*, FILE*);
int getNAtleti(Lista*);
int delAtleta(Lista*, Atleta*);

int caricaAtleti(); // TODO

#endif
