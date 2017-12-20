#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

typedef struct _Item* Item;
typedef char* Key;
typedef int Value;

// get
Value ITEMgetValue(Item);
Key ITEMgetKey(Item);

// visualizzazione
void ITEMdisplay(FILE*, Item);

//confronto
int KEYeq(Key, Key);
int KEYgreater(Key, Key);
int KEYless(Key, Key);

Item ITEMsetVoid();
Item ITEMnew(Key, Value);
void ITEMfree(Item);
int ITEMmaxKeyLen();


#endif