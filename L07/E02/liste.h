#ifndef LISTE_H_INCLUDED
#define LISTE_H_INCLUDED

typedef struct _Lista Lista;
typedef struct node node_t, *link;

link getHead(Lista *l);
link getNext(link x);
link getVal(link x);

Lista *initList(size_t elSize);
link newNode(void *val, size_t elSize, link prev, link next, char *mode);

// nelle addHead e addTail con mode si intende la stringa "c" oppure "p"
// con "c" viene COPIATO ciò che è puntato da val
// con "p" viene PUNTATO ciò che è puntato da val
int addHead(Lista *l, void *val, char *mode);
int addTail(Lista *l, void *val, char *mode);

//int delNode(Lista *l, void *key, void *(*getKeyFunc)(void*), int (*keyEqualFunc)(void*, void*));
int delNode(Lista *l, link x);

#endif // LISTE_H_INCLUDED
