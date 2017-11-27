#include "mie.h"

#ifndef _ATLETI // codice da eseguire se non si è già incluso "atleti.h"
#define _ATLETI


typedef struct atleta_s atleta_t;

typedef struct node node_t, *link;

link newNode(atleta_t, link);
link addTailNode(link, atleta_t);
link delNode(link, char*);
link cercaAtleta(link, char*);
link inputCercaAtleta(link);
void stampaAnagrafica(link, FILE*);
void stampaAtleta(link, FILE*);

#endif // _ATLETI
