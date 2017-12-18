#ifndef ES_H_INCLUDED
#define ES_H_INCLUDED

#define MAX_ESERCIZIO 50

#include "liste.h"

typedef struct esercizio_s esercizio_t;
typedef struct pianoEs_s pianoEs_t;
typedef struct _tabellaEs *tabellaEs;

esercizio_t *linkEs(tabellaEs, char*);
void stampaTuttiEs(Lista, FILE*);
int modificaPianoEsByName(Lista, char*, int, int);
int cancellaPianoEsByName(Lista, char*);
int aggiungiEs(Lista, tabellaEs, char*, int, int);

tabellaEs newEsCollection();
void caricaEsercizi(tabellaEs, FILE*);
void caricaPianoEsercizi(Lista, tabellaEs, FILE*);
Lista newListaEs();
#endif
