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



pianoEs_t *getPianoEsFromNode(link);
esercizio_t *linkEs(tabellaEs*, char*);
void stampaPianoEs(pianoEs_t*, FILE*);
void modificaPianoEs(pianoEs_t*, int, int);
void cancellaPianoEs(Lista*, link);
int aggiungiEs(Lista*, tabellaEs*, char*, int, int);

#endif
