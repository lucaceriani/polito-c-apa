#ifndef BST_H_INCLUDED
#define BST_H_INCLUDED

typedef struct _BSTnode* link;
typedef struct _BST* BST;

BST BSTreadPreorder(FILE*);
void BSTwritePreorder(FILE*, BST);
int BSTcount(BST, int, int);
int BSTdistance(BST, Key, Key);
void BSTmirror(BST);
void BSTfree(BST);

#endif