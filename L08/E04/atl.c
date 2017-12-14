#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "atl.h"

#include "es.h"
#include "liste.h"

#define MAX_NOME 25
#define LUNG_CODICE 5

static Atleta *getAtletaFromNode(link x) {
    return (Atleta*)(getVal(x));
}

int getNAtleti(Lista *l) {
    return getN(l);
}

int getOreAtleta(Atleta *a) {
    return a->ore;
}

char* getCodiceAtleta(Atleta *a) {
    return a->codice;
}

Lista *getListaEsercizi(Atleta *atl) {
    return atl->listaEs;
}

static link cercaAtletaByAtlP(Lista *l, Atleta *atl) {
    link x; 
    for (x=getHead(l); x!=NULL; x=getNext(x)) {
        if ((void*)atl==getVal(x)){
            return x;
        }
    }
    return NULL;
}

void modificaOreAtl(Atleta *a, int ore) {
    a->ore=ore;
}

Atleta *newAtleta() {
    return (Atleta*)malloc(sizeof(Atleta));
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

static void strcatMia(char *dst, char *src1, char *src2) {
    strcpy(dst, "");
    strcpy(dst, src1);
    strcat(dst, src2);
}

static void aggiungiAtleta(Lista *l, Atleta *atl) {
    atl->cognomenome=(char*)malloc(strlen(atl->nome)+strlen(atl->cognome)+1);
    strcatMia(atl->cognomenome, atl->cognome, atl->nome);
    addTail(l, atl, "c");
}

void aggiungiAtletaByPar(Lista *l, char *codice, char *nome, char *cognome, 
                         char *categoria, char *data, int ore) {
    
    Atleta a;
    // non c'è bisogno di fare la copia in memoria delle cose perché
    // ci pensa già aggiungiAtleta.
    a.codice=strdup(codice);
    a.nome=strdup(nome);
    a.cognome=strdup(cognome);
    a.categoria=strdup(categoria);
    a.data=strdup(data);
    a.ore=ore;
    a.listaEs=initList(sizeof(pianoEs_t));
    aggiungiAtleta(l, &a);
    
}

int cancellaAtleta(Lista *l, Atleta *atl) {
    link x=cercaAtletaByAtlP(l,atl);
    
    if (x==NULL) return 0;
    // else
    free(atl->codice);
    free(atl->nome);
    free(atl->cognome);
    free(atl->cognomenome);
    free(atl->categoria);
    free(atl->data);
    delNode(l, x); return 1;
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
    fprintf(fp, "%s %s %s %s %s %d\n", a->codice, a->nome, a->cognome,
        a->categoria, a->data, a->ore);
    
    stampaTuttiEs(getListaEsercizi(a), fp);
}

void stampaAnagrafica(Lista *l, FILE *fp) {
    link h=getHead(l);
    fprintf(fp, "%d\n", getNAtleti(l));
    for (; h!=NULL; h=getNext(h)) stampaAtleta(getAtletaFromNode(h), fp);
}

int eserciziCaricatiAtl(Atleta *atl) {
    return !isEmpty(getListaEsercizi(atl));
}

void caricaAtleti(Lista *l, FILE *fp) {
    Atleta tmp;
    int i=0,n;
    
    if (fscanf(fp, "%d\n", &n)!=1) exit(2); // controllo prima riga
    
    tmp.codice=malloc(LUNG_CODICE+1);
    tmp.nome=malloc(MAX_NOME+1);
    tmp.cognome=malloc(MAX_NOME+1);
    tmp.categoria=malloc(MAX_NOME+1);
    tmp.data=malloc(11);

    while(i++<n && fscanf(fp, "%s %s %s %s %s %d\n", tmp.codice,
        tmp.cognome, tmp.nome, tmp.categoria, tmp.data,
        &tmp.ore)==6) {

        aggiungiAtletaByPar(l, tmp.codice, tmp.nome, tmp.cognome, tmp.categoria,
                            tmp.data, tmp.ore);
    }
}

Lista *newAtlCollection() {
    return initList(sizeof(Atleta));
}
