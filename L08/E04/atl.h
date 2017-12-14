#ifndef ATL_H_INCLUDED
#define ATL_H_INCLUDED

#include "liste.h"

#define MAX_ESERCIZIO 50
#define MAX_NOME 25
#define LUNG_CODICE 5

typedef struct {
    char *codice;
    char *nome;
    char *cognome;
    char *categoria;
    char *data;
    char *cognomenome;
    int ore;
    Lista *listaEs;
} Atleta; // TODO: adt 1 classe

// funzioni di visualizzazione
void stampaAtleta(Atleta*, FILE*);
void stampaPerCategoria(Lista*);
void stampaAnagrafica(Lista*, FILE*);


// funzioni get
Lista *getListaEsercizi(Atleta*);
int getNAtleti(Lista*);
int getOreAtleta(Atleta*);
char *getCodiceAtleta(Atleta*);

// operazioni sugli atleti
Atleta *newAtleta();
Lista* newAtlCollection();
Atleta *cercaAtleta(Lista*, char*);
int cancellaAtleta(Lista*, Atleta*);
int eserciziCaricatiAtl(Atleta*);
void caricaAtleti(Lista*, FILE*);
void aggiungiAtletaByPar(Lista*, char*, char*, char*, char*, char*, int);
void modificaOreAtl(Atleta*, int);

#endif
