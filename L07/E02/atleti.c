#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_NOME 25
#define LUNG_CODICE 5

typedef struct atleta_s{
    char nome[MAX_NOME+1];
    char cognome[MAX_NOME+1];
    char cognomenome[2*MAX_NOME+1];
    char categoria[MAX_NOME+1];
    char codice[LUNG_CODICE+1];
    char data[11];
    int ore;
} atleta_t;

typedef struct node {
    atleta_t val;
    struct node *next;
} node_t, *link;

link newNode(atleta_t val, link next) {
    link x = malloc(sizeof(node_t));
    if (x==NULL) return NULL;
    else {
        x->val = val;
        x->next = next;
    }
    return x;
}

link addTailNode(link h, atleta_t val) {
    link x;
    if (h==NULL)
        return newNode(val, NULL);
    for (x=h; x->next!=NULL; x=x->next);
    x->next = newNode(val, NULL);
    return h;
}

link delNode(link h, char *k) {
    link x, p;
    if (h == NULL)
        return 0;
    for (x=h, p=NULL; x!=NULL; p=x, x=x->next) {
        if (_comp(x->val.codice, k)==0) {
            if (x==h)
                h = x->next;
            else
                p->next = x->next;
            free(x);
            break;
        }
    }
    return h;
}

void stampaAtleta(link h, FILE *fp) {
    atleta_t a=h->val;
    fprintf(fp, "%s %s %s %s %s %d\n", a.codice, a.nome, a.cognome,
        a.categoria, a.data,a.ore);
}

void stampaAnagrafica(link h, FILE *fp) {
    for (; h!=NULL; h=h->next) stampaAtleta(h, fp);
}

link cercaAtleta(link h, char *s) {
    // cerca atleta è una funzione intelligente che in base alle caratteristiche
    // della stringa passata come chiave cerca o nel codice o nel cogome
    // (anche parziale).

    // prima di tutto cerco di capire se so tratta di un codice. Un codice deve
    // avere tre caratteristiche:
    //      1 - avere cinque caratteri;
    //      2 - essere nel formato %c%d
    //      3 - tolower(c) == a;
    // se non si riscontrano queste due caratteristiche si presuppone che
    // la ricerca sia fatta per cognome.
    char c;
    int d, isCodice=0;

    if (strlen(s)==LUNG_CODICE && sscanf(s, "%c%d", &c,&d) && tolower(c)=='a') {
        isCodice=1;
    }
        for (; h!=NULL; h=h->next) {
            if (isCodice) {
                if (_comp(h->val.codice, s)==0) return h;
            } else {
                if (startsWith(h->val.cognomenome, s)==1) return h;
            }
        }
    // se non sono ritornato prima vuol dire che non ho trovato niente
    return NULL;
}



link inputCercaAtleta(link h) {
    char c[MAX_NOME+1];
    link p;
    printf("Codice o cognome parziale dell'atleta: ");
    scanf("%s", c);
    if ((p=cercaAtleta(h, c))!=NULL) {
        stampaAtleta(p, stdout);
        return p;
    } else {
        puts("Atleta non trovato");
        return NULL;
    }
}




