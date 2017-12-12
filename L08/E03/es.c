#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "liste.h"
#include "es.h"

pianoEs_t *getPianoEsFromNode(link x) {
    return (pianoEs_t*)(getVal(x));
}

void stampaPianoEs(pianoEs_t *p, FILE *fp) {
    fprintf(fp, "\t%s %s %s", p->es->nomeEs, p->es->catEs, p->es->tipoEs);
    fprintf(fp, " %d set %d rptz\n", p->set, p->rep);
}

void modificaPianoEs(pianoEs_t* piano, int set, int rep) {
    piano->set=set;
    piano->rep=rep;
}

void cancellaPianoEs(Lista *l, link x) {
    delNode(l, x);
}

int aggiungiEs(Lista *l, tabellaEs *tabEs, char *nome, int set, int rep) {
    pianoEs_t p;
    // se l'esercizio non esiste ritorno zero, nel frattempo lo aggiungo a p.es
    if ((p.es=linkEs(tabEs, nome))==NULL) return 0;
    p.set=set;
    p.rep=rep;
    
    // se tutto è a posto aggiungo l'esercizio in coda
    addTail(l, (void*)(&p), "c");
        
}

esercizio_t *linkEs(tabellaEs* esercizi, char* nomeEs) {
    int i;
    for (i=0; i<esercizi->nEs; i++) {
        if (strcasecmp(nomeEs, esercizi->vettEs[i].nomeEs)==0)
            return &(esercizi->vettEs[i]);
    }
    return NULL;
}
