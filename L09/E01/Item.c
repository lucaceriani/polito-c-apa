#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Item.h"

#define MAX_KEY 10

struct _Item {
    Key key;
    Value x;
};

int ITEMmaxKeyLen() {
    return MAX_KEY;
}

Value ITEMgetValue(Item i) {
    return i->x;
}

Key ITEMgetKey(Item i) {
    return i->key;
}

void ITEMdisplay(FILE *fp, Item i) {
    fprintf(fp, "%s %d", ITEMgetKey(i), ITEMgetValue(i));
}

Item ITEMsetVoid() {
    Item i=malloc(sizeof *i);
    i->key=strdup("");
    i->x=0;
    return i;
}

Item ITEMnew(Key k, Value v) {
    Item i=malloc(sizeof *i);
    i->key=strdup(k);
    i->x=v;
    return i;
}

int KEYless(Key i, Key j) {
    return strcasecmp(i, j)<=-1;
}

int KEYgreater(Key i, Key j) {
    return strcasecmp(i, j)>=1;
}

int KEYeq(Key i, Key j) {
    return strcasecmp(i, j)==0;
}

void ITEMfree(Item i) {
    free(i->key);
    free(i);
}
