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


link cercaAtleta(link h, char *s) {
    // cerca atleta è una funzione intelligente che in base alle caratteristiche
    // della stringa passata come chiave cerca o nel codice o nel cogome
    // (anche parziale).

    // prima di tutto cerco di capire se si tratta di un codice. Un codice deve
    // avere tre caratteristiche:
    //      1 - avere cinque caratteri;
    //      2 - essere nel formato %c%d
    //      3 - tolower(c) == a;
    // se non si riscontrano queste due caratteristiche si presuppone che
    // la ricerca sia fatta per cognome.

    // fpComp è il puntatore alla funzione di comparazione che cambia
    // a seconda che sia un codice o un cognome
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
            if (fpComp((isCodice?tmp->codice:tmp->cognomenome), s)==0) return h;
        }

    // se non sono ritornato prima vuol dire che non ho trovato niente
    return NULL;
}

void stampaAtleta(link h, FILE *fp) {
    link p;
    Atleta *a=getAtletaFromNode(h);
    fprintf(fp, "%s %s %s %s %s %d\n", a->codice, a->nome, a->cognome,
        a->categoria, a->data, a->ore);
    
    // controllo se ci sono degli esercizi caricati, in caso li stampo
    p=getHead(a->listaEs);
    if (p!=NULL) {
        for (; p!=NULL; p=getNext(p))
        stampaPianoEs(getPianoEsFromNode(p), stdout);
    }
}
