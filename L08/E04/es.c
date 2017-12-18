#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "liste.h"
#include "es.h"

struct esercizio_s{
    char *nomeEs;
    char *catEs;
    char *tipoEs;
};

struct pianoEs_s{
    esercizio_t *es;
    char nomeEs[MAX_ESERCIZIO+1];
    int set,rep;
};

struct _tabellaEs {
    esercizio_t *vettEs;
    int nEs;
};

static pianoEs_t *getPianoEsFromNode(link x) {
    return (pianoEs_t*)(getVal(x));
}

static void stampaEs(pianoEs_t *p, FILE *fp) {
    fprintf(fp, "\t%s %s %s", p->es->nomeEs, p->es->catEs, p->es->tipoEs);
    fprintf(fp, " %d set %d rptz\n", p->set, p->rep);
}

void stampaTuttiEs(Lista l, FILE *fp) {
    link p=getHead(l);
    if (p!=NULL) {
        for (; p!=NULL; p=getNext(p))
            stampaEs(getPianoEsFromNode(p), fp);
    }
}

static void modificaPianoEs(pianoEs_t* piano, int set, int rep) {
    piano->set=set;
    piano->rep=rep;
}

static void cancellaPianoEs(Lista l, link x) {
    delNode(l, x);
}

int aggiungiEs(Lista l, tabellaEs tabEs, char *nome, int set, int rep) {
    pianoEs_t p;
    // se l'esercizio non esiste ritorno zero, nel frattempo lo aggiungo a p.es
    if ((p.es=linkEs(tabEs, nome))==NULL) return 0;
    p.set=set;
    p.rep=rep;
    
    // se tutto è a posto aggiungo l'esercizio in coda
    addTail(l, (void*)(&p), "c");
    return 1;
}

esercizio_t *linkEs(tabellaEs esercizi, char* nomeEs) {
    int i;
    for (i=0; i<esercizi->nEs; i++) {
        if (strcasecmp(nomeEs, esercizi->vettEs[i].nomeEs)==0)
            return &(esercizi->vettEs[i]);
    }
    return NULL;
}

int modificaPianoEsByName(Lista l, char *nome, int set, int rep) {
    link p;
    pianoEs_t *tmpPianoP;
    for (p=getHead(l); p!=NULL; p=getNext(p)) {
        tmpPianoP=getPianoEsFromNode(p);
        // se becco l'esercizio che vogio modificare tramite
        // la strcmp lo modifico con i nuovi valori appena acquisiti
        if (strcasecmp(tmpPianoP->es->nomeEs, nome)==0) {
            modificaPianoEs(tmpPianoP, set, rep);
            return 1;
        }
    }
    return 0;
}

tabellaEs newEsCollection() {
    return (tabellaEs)malloc(sizeof(struct _tabellaEs));
}

Lista newListaEs() {
    return initList(sizeof(pianoEs_t));
}

void caricaEsercizi(tabellaEs esercizi, FILE *fp) {
    int n=0, i=0;
    esercizio_t tmpEs;
    char nome[MAX_ESERCIZIO+1], cat[MAX_ESERCIZIO+1], tipo[MAX_ESERCIZIO+1];
    
    if (fscanf(fp, "%d", &n)!=1) exit(2);
    // alloco il vettore degli esercizi
    esercizi->vettEs=malloc(n*sizeof(esercizio_t));
    esercizi->nEs=n;

    while (i<n && fscanf(fp, "%s %s %s",nome,cat,tipo)==3){
        tmpEs.nomeEs=strdup(nome);
        tmpEs.catEs=strdup(cat);
        tmpEs.tipoEs=strdup(tipo);
        esercizi->vettEs[i++]=tmpEs;
    }
}

void caricaPianoEsercizi(Lista l, tabellaEs esercizi, FILE *fp) {
    pianoEs_t tmpPiano;
    while (fscanf(fp, "%s %d %d",tmpPiano.nomeEs, &tmpPiano.set,
           &tmpPiano.rep)==3) {
        aggiungiEs(l, esercizi, tmpPiano.nomeEs, tmpPiano.set, tmpPiano.rep);
    }
}

int cancellaPianoEsByName(Lista l, char *nome) {
    link p;
    pianoEs_t *tmpPianoP;
    
    for (p=getHead(l); p!=NULL; p=getNext(p)) {
        tmpPianoP=getPianoEsFromNode(p);
        if (strcasecmp(tmpPianoP->es->nomeEs, nome)==0) {
            cancellaPianoEs(l, p);
            return 1;
        }
    }
    return 0;
}