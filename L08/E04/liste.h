#ifndef LISTE_H_INCLUDED
#define LISTE_H_INCLUDED

typedef struct _Lista Lista;
typedef struct node node_t, *link;

link getHead(Lista*);
int getN(Lista*);
link getNext(link);
link getVal(link);

Lista *initList(size_t);
// val, sizeof element, prev, next, mode
link newNode(void*, size_t, link, link, char*);

// nelle addHead e addTail con mode si intende la stringa "c" oppure "p"
// con "c" viene COPIATO ciò che è puntato da val
// con "p" viene PUNTATO ciò che è puntato da val
int addHead(Lista*, void*, char*);
int addTail(Lista*, void*, char*);

//int delNode(Lista *l, void *key, void *(*getKeyFunc)(void*), int (*keyEqualFunc)(void*, void*));
int delNode(Lista*, link);

int isEmpty(Lista*);

#endif // LISTE_H_INCLUDED
