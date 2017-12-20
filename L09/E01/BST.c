#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "Item.h"
#include "BST.h"

struct _BSTnode {
    Item item;
    link r,l;
};

struct _BST {
    link root, z;
};

static link BSTnewNode(Item item, link l, link r) {
    link x=malloc(sizeof *x);
    x->item=item;
    x->l=l;
    x->r=r;
    return x;
}

static BST BSTinit() {
    BST bst=malloc(sizeof *bst);
    bst->root=(bst->z=BSTnewNode(ITEMsetVoid(), NULL, NULL));
    return bst;
}

static int isComplete(link node, link z) {
    return (node->l!=z && node->r!=z);
}

static int isLeaf(link node, link z) {
    return (node->l==z && node->r==z);
}

static int hasRightChild(link node, link z) {
    return node->r!=z;
}


static int hasLeftChild(link node, link z) {
    return node->l!=z;
}

static link BSTinsertFromFile(FILE *fp, link node, link z) {
    
    int r, l;
    Key k=malloc(ITEMmaxKeyLen()*(sizeof *k));
    Value v;
    
    // controllo che possa ancora leggere qualcosa
    if (fscanf(fp, "%s %d %d %d", k, &v, &l, &r)!=4) return NULL;
    
    node->item=ITEMnew(k, v);
    
    if (l) { // se c'è il nodo sinistro
        node->l = BSTnewNode(ITEMsetVoid(), z, z);
        node->l = BSTinsertFromFile(fp, node->l, z);
    }
    if (r) { // se c'è il nodo destro
        node->r = BSTnewNode(ITEMsetVoid(), z, z);
        node->r = BSTinsertFromFile(fp, node->r, z);
    }
    
    free(k);
    return node;
    
}

static void BSTwriteToFile(FILE *fp, link node, link z) {
    
    // stampo il nodo corrente
    ITEMdisplay(fp, node->item);
    fprintf(fp, " %d %d\n", hasLeftChild(node,z), hasRightChild(node,z));
    
    if (hasLeftChild(node,z)) { // se non sono alla fine, proseguo...
        BSTwriteToFile(fp, node->l ,z);
    }
    if (hasRightChild(node,z)) {
        BSTwriteToFile(fp, node->r ,z);        
    }
}

BST BSTreadPreorder(FILE *fp) {
    BST bst=BSTinit();
    bst->root=BSTinsertFromFile(fp, bst->root, bst->z);
    return bst;
}

void BSTwritePreorder(FILE *fp, BST bst) {
    BSTwriteToFile(fp, bst->root, bst->z);
}



static int BSTcountR(link node, link z,  int cnt, int currLvl, int l1, int l2) {
     
    // condizione di terminazione
    if (isLeaf(node, z)) return cnt;
    
    // conteggio o meno del nodo in cui sono se sono compreso tra i due livelli
    if (isComplete(node, z) && currLvl>=l1 && currLvl<=l2) cnt++;
    
    // ricorro scendendo di un livello
    if (hasLeftChild(node,z))
        cnt=BSTcountR(node->l, z, cnt, currLvl+1, l1, l2);

    if (hasRightChild(node,z))
        cnt=BSTcountR(node->r, z, cnt, currLvl+1, l1, l2);
        
    return cnt;
}

int BSTcount(BST bst, int l1, int l2) {
    int cnt;
    
    cnt=BSTcountR(bst->root, bst->z, 0, 0, l1, l2);
    
    return cnt;
}

// funzione per trovare la più vicina radice in comune
static link leastCommonRoot(link node, link z, Key k1, Key k2) {
    
    // condizione di terminazione
    if (isLeaf(node, z)) return NULL;
    

    // se stanno tutte e due a dx o a sx del nodo corrente non mi trovo
    // sul leastCommonRoot quindi ricorro
    
    if (KEYless(k1, ITEMgetKey(node->item)) && KEYless(k2, ITEMgetKey(node->item))) {
        return leastCommonRoot(node->l, z, k1, k2);
    } else if (KEYgreater(k1, ITEMgetKey(node->item)) && KEYgreater(k2, ITEMgetKey(node->item))) {
        return leastCommonRoot(node->r, z, k1, k2);
    }

    return node;
    
}
// funzione che misura la distanza della chiave da un nodo dato node
static int BSTdistanceR(link node, link z, Key k, int dist) {
    // caso terminale sono sul nodo con la mia chiave oppure sono in una foglia
    if (KEYeq(ITEMgetKey(node->item), k)) return dist;
    if (isLeaf(node, z)) return -INT_MAX/2;
    
    // altrimenti ricorro
    if (KEYless(k, ITEMgetKey(node->item)))
        dist=BSTdistanceR(node->l, z, k, dist+1);
    if (KEYgreater(k, ITEMgetKey(node->item)))
        dist=BSTdistanceR(node->r, z, k, dist+1);
    
    return dist;
}

int BSTdistance(BST bst, Key k1, Key k2) {
    // controllo che le due chiavi non siano uguali
    if (KEYeq(k1,k2)) return 0;
    
    link lcr=leastCommonRoot(bst->root, bst->z, k1, k2);
    int d1=0, d2=0;
    
    d1=BSTdistanceR(lcr, bst->z, k1, 0);
    d2=BSTdistanceR(lcr, bst->z, k2, 0);
    
    return d1+d2;
    
}

void BSTmirrorR(link node, link z) {
    link tmp;
    
    // condizione di terminazione: sono su una foglia
    if (isLeaf(node,z)) return;
    
    // scambio l con r
    tmp=node->r;
    node->r=node->l;
    node->l=tmp;
    
    // ricorro
    if (hasRightChild(node,z))
        BSTmirrorR(node->r, z);
    if (hasLeftChild(node,z))
        BSTmirrorR(node->l, z);
}

void BSTmirror(BST bst) {
    BSTmirrorR(bst->root, bst->z);
}

static void freeNode(link node) {
    ITEMfree(node->item);
    free(node);
}

void BSTfreeR(link node, link z) {
    // condizione di terminazione sono su una foglia
    if (isLeaf(node,z)) freeNode(node);
    
    // altrimenti ricorro
    if (hasLeftChild(node,z))
        BSTfreeR(node->l, z);
    if (hasRightChild(node,z))
        BSTfreeR(node->r, z);
    
}

void BSTfree(BST bst) {
    
}