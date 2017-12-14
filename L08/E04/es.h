#ifndef ES_H_INCLUDED
#define ES_H_INCLUDED

#define MAX_ESERCIZIO 50

#include "liste.h"

typedef struct {
    char nomeEs[MAX_ESERCIZIO+1];
    char catEs[MAX_ESERCIZIO+1];
    char tipoEs[MAX_ESERCIZIO+1];
} esercizio_t;

typedef struct {
    esercizio_t *es;
    char nomeEs[MAX_ESERCIZIO+1];
    int set,rep;
} pianoEs_t;

typedef struct {
    esercizio_t *vettEs;
    int nEs;
} tabellaEs;



esercizio_t *linkEs(tabellaEs*, char*);
void stampaTuttiEs(Lista*, FILE*);
int modificaPianoEsByName(Lista*, char*, int, int);
int cancellaPianoEsByName(Lista*, char*);
int aggiungiEs(Lista*, tabellaEs*, char*, int, int);

tabellaEs *newEsCollection();
void caricaEsercizi(tabellaEs*, FILE*);
void caricaPianoEsercizi(Lista*, tabellaEs*, FILE*);

#endif
