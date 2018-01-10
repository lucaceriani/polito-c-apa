#ifndef BST_H_DEFINED
#define BST_H_DEFINED

#include "Item.h"

typedef struct  binarysearchtree *BST;

BST     BSTinit();
int     BSTcount(BST);
int     BSTempty(BST);
Item    BSTmin(BST);
Item    BSTmax(BST);
void    BSTinsert_leafI(BST, Item);
void    BSTinsert_leafR(BST, Item);
void    BSTinsert_root(BST, Item);
Item    BSTsearch(BST, Key);
void    BSTdelete(BST, Key);
Item    BSTselect(BST, int);
void    BSTsortinorder(BST);
void    BSTsortpreorder(BST);
void    BSTsortpostorder(BST);
Item    BSTsucc(BST, Key);
Item    BSTpred(BST, Key);

#endif
