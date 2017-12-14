#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "liste.h"

struct node {
    void *val;
    struct node *next;
    struct node *prev;
};

struct _Lista {
    int n;
    size_t elementSize;
    link head;
    link tail;
};

int getN(Lista *l) {
    return l->n;
}

link getHead(Lista *l) {
    return l->head;
}

link getNext(link x) {
    return x->next;
}

link getPrev(link x) {
    return x->prev;
}

link getVal(link x) {
    return x->val;
}

int isEmpty(Lista *l) {
    return (getHead(l)==NULL)?1:0;
}

Lista *initList(size_t elSize) {
    Lista *l;

    // alloco
    l=(Lista*)malloc(sizeof(Lista));

    // set to zero
    l->n=0;
    l->elementSize=elSize;
    l->head=NULL;
    l->tail=NULL;
    return l;
}

link newNode(void *val, size_t elSize, link prev, link next, char *mode) {
    link x = malloc(sizeof(node_t));
    if (x==NULL) return NULL; // controllo sul successo della malloc
    else {
        // mode è la modalità concui si collega il valore di val
        // può essere "c" per copiare/duplicare il valore oppure
        // "p" per puntare dove punta val.
        if (tolower(mode[0])=='p') x->val=val;
        else if (tolower(mode[0])=='c'){
            // uso memcpy per duplicare il contetnuto del puntatore
            // la dimensione è data da elSize che è la dimensione
            // della struttura che bisogna allocare
            x->val=malloc(elSize);
            memcpy(x->val, val, elSize);
        } else return NULL;

        x->prev=prev;
        x->next=next;
    }
    return x;
}

int addHead(Lista *l, void *val, char* mode) {
    link oldFirstItem;
    if (l->head==NULL) {
        // se la testa è vuota vuole dire che la lista è stata appena
        // inizializzata e quindi procedo a creare un nuovo nodo da
        // collegare alla testa
        l->tail=l->head=newNode(val, l->elementSize, NULL, NULL, mode);
        return 1;
    } else {
        oldFirstItem=l->head;
        l->head=newNode(val, l->elementSize, NULL, oldFirstItem, mode);
        return 0;
    }
    l->n++;
}

int addTail(Lista *l, void *val, char *mode) {
    link oldLastItem;
    if (l->head==NULL && l->tail==NULL) { // se la lista è vuota
        l->head=l->tail=newNode(val, l->elementSize, NULL, NULL, mode);
        l->n++;
        return 1;
    } else {
        // prendo la tail attuale e me la salvo poi collego la tail a un new
        // node e il prev di questo newnode diventa la vecchia tail, cioè
        // il penultimo nodo
        oldLastItem=l->tail;
        l->tail=newNode(val, l->elementSize, oldLastItem, NULL, mode);
        // aggiorno il next del precedente
        oldLastItem->next=l->tail;
        l->n++;
        return 0;
    }
}

static int freeNode(link p) {
    free(p->val);
    free(p);
    return 0;
}

int delNode(Lista *l, link x) {

    if (l->head == l->tail) {
        // cancellazione di un elemento nella lista
        freeNode(x);
        l->head = l->tail = NULL;
    } else if (x==l->head) {
        // cancellazione in testa con più di un elemento
        // imposto il valore prev del secondo elemento a NULL
        x->next->prev = NULL;
        // la testa diventa il secondo elemento
        l->head=x->next;
        freeNode(x);
    } else if (x==l->tail) {
        // cancellazione in coda
        // il valore next del precedente di x diventa NULL
        x->prev->next=NULL;
        l->tail=x->prev;
        freeNode(x);
    } else {
        // cancellazione di un elemento in mezzo alla lista
        // il successivo del precedente diventa il successivo di x
        x->prev->next=x->next;
        // il precedente del successivo diventa il precedente di z
        x->next->prev=x->prev;
        freeNode(x);
    }
    return l->n--;
}
