#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "atl.h"

#include "es.h"
#include "liste.h"


Atleta *getAtletaFromNode(link x) {
    return (Atleta*)(getVal(x));
}

int getNAtleti(Lista *l) {
    return getN(l);
}

static int startsWith(const char *a, const char *b) {
    // case unsensitive
    int i, n=0;
    // voglio proseguire il confronto fino all'ultima lettera
    // di b (che dorevbbe essere la più corta)

    n=strlen(b);
    if (strlen(a)<n) return -1;

    for (i=0; i<n; i++) if (tolower(a[i])!=tolower(b[i])) return -1;

    return 0;
}

int delAtleta(Lista *l, Atleta *atl) {
    link x; 
    for (x=getHead(l); x!=NULL; x=getNext(x)) {
        if ((void*)atl==getVal(x)){
            delNode(l, x);
            return 1;
        }
    }
    return 0;
}

void stampaPerCategoria(Lista *atleti) {
    int i=0,j=0;
    link p,q;
    Atleta *tmpAtletaP, *tmpAtletaQ;
    int n=getNAtleti(atleti);
    int *printed=calloc(n, sizeof(int));
    // controllo in caso mi si chieda di scrivere una lista vuota
    if (n<=0) return;
    
    
    p=getHead(atleti);
    tmpAtletaP=getAtletaFromNode(p);

    // per far funzionare questa cosa mi sono fatto un vettore delle occorrenze
    // dove tengo tutti gli atleti che ho già stampato
    // N.B. gli indici i e j nei for!!
    for (; p!=NULL; p=getNext(p), i++) {
        tmpAtletaP=getAtletaFromNode(p);
        if (printed[i]) continue; // se l'ho già stampato passo al prossimo
        
        printf(" -> %s\n", tmpAtletaP->categoria);
        for (q=p, j=i; q!=NULL; q=getNext(q), j++) {
            tmpAtletaQ=getAtletaFromNode(q);
            if (strcasecmp(tmpAtletaQ->categoria, tmpAtletaP->categoria)==0) {
                stampaAtleta(tmpAtletaQ, stdout);
                printed[j]=1; // salvo il fatto che l'ho stampato
            }
        }
        
    }
}

Atleta *cercaAtleta(Lista *l, char *s) {
    /* cerca atleta è una funzione intelligente che in base alle caratteristiche
     * della stringa passata come chiave cerca o nel codice o nel cogome
     * (anche parziale).

     * prima di tutto cerco di capire se si tratta di un codice. Un codice deve
     * avere tre caratteristiche:
     *      1 - avere cinque caratteri;
     *      2 - essere nel formato %c%d
     *      3 - tolower(c) == a;
     * se non si riscontrano queste due caratteristiche si presuppone che
     * la ricerca sia fatta per cognome.

     * fpComp è il puntatore alla funzione di comparazione che cambia
     * a seconda che sia un codice o un cognome
     */
    link h=getHead(l);
    int (*fpComp)(const char*, const char*);
    char c;
    int d, isCodice=0;
    Atleta *tmp;

    if (strlen(s)==LUNG_CODICE && sscanf(s, "%c%d", &c,&d) && tolower(c)=='a') {
        // se è un codice
        fpComp=strcasecmp;
        isCodice=1;
    } else {
        // se è un cognome
        fpComp=startsWith;
    }
        for (; h!=NULL; h=getNext(h)){
            tmp=getAtletaFromNode(h);
            if (fpComp((isCodice?tmp->codice:tmp->cognomenome), s)==0)
                return getAtletaFromNode(h);
        }

    // se non sono ritornato prima vuol dire che non ho trovato niente
    return NULL;
}

void stampaAtleta(Atleta *a, FILE *fp) {
    link p;
    fprintf(fp, "%s %s %s %s %s %d\n", a->codice, a->nome, a->cognome,
        a->categoria, a->data, a->ore);
    
    // controllo se ci sono degli esercizi caricati, in caso li stampo
    p=getHead(a->listaEs);
    if (p!=NULL) {
        for (; p!=NULL; p=getNext(p))
        stampaPianoEs(getPianoEsFromNode(p), stdout);
    }
}

void stampaAnagrafica(Lista *l, FILE *fp) {
    link h=getHead(l);
    fprintf(fp, "%d\n", getNAtleti(l));
    for (; h!=NULL; h=getNext(h)) stampaAtleta(getAtletaFromNode(h), fp);
}
