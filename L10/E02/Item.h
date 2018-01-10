#ifndef _DATO_INCLUDED
#define _DATO_INCLUDED

#define ID_LEN 3

typedef struct _item* Item;
typedef char *Key;

Item  ITEMscan();
void  ITEMshow(Item data);
int   ITEMcheckvoid(Item data);
Item  ITEMsetvoid();
int   ITEMless(Item data1, Item data2);
Item  ITEMnew(char*, int, int, int);
Key   KEYscan();
int   KEYcompare(Key k1, Key k2);
Key   KEYget(Item data);

// mie
void  ITEMaggiornaStazione(Item, int);
int   ITEMgetNoleggio(Item);
void  ITEMsetNoleggio(Item, int);

#endif

