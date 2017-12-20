/*
 * FILE: client.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Item.h"
#include "BST.h"

#define MAXS 11

int main(int argc, char **argv) {
    FILE *in, *out;
    BST tree;
    int l1, l2;
    char k1[MAXS], k2[MAXS];

    in = fopen(argv[1], "r");
    if (in==NULL) {
        printf("errore in apertura di %s\n", argv[1]);
        return 1;
    }
    tree = BSTreadPreorder(in);
    fclose(in);

    printf("BST acquisito - stampa in preorder:\n");

    BSTwritePreorder(stdout,tree);
    
    // invocazione delle funzioni richieste

    // conteggio nodi completi tra due livelli 
    printf("livelli tra cui contare nodi: ");
    scanf("%d%d", &l1, &l2);
    printf("nodi completi tra i livelli %d e %d: %d\n", l1, l2, BSTcount(tree,l1,l2));

    // distanza tra due nodi /
    printf("chiavi dei nodi tra cui misurare distanza: ");
    scanf("%s%s", k1, k2);
    printf("distanza tra i nodi con chiave %s e %s: %d\n",  k1, k2, BSTdistance(tree,k1,k2));

    // inversione ordine 
    BSTmirror(tree);

    // salva su secondo file
    out = fopen(argv[2], "w");
    if (out==NULL) {
        printf("errore in apertura di %s\n", argv[2]);
        return 1;
    }
    
    BSTwritePreorder(out,tree);
    
    BSTfree(tree);

    return 0;
}
