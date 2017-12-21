#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

typedef struct _Item* Item;

Item ITEMnew(char*, int);
int ITEMgreater(Item, Item);
int ITEMless(Item, Item);
void ITEMdisplay(FILE*, Item, const char*);
int ITEMgetPoints(Item);
char *ITEMgetName(Item);
void ITEMfree(Item);

#endif
