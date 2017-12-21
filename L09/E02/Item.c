#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Item.h"

struct _Item {
    char *name;
    int points;
        
};

int ITEMgetPoints(Item i) {
    return i->points;
}

char *ITEMgetName(Item i) {
    return i->name;
}

void ITEMfree(Item i) {
    free(i->name);
    free(i);
}

Item ITEMnew(char *name, int points) {
    Item i=malloc(sizeof *i);
    i->name=strdup(name);
    i->points=points;
    return i;
}

int ITEMgreater(Item i, Item j) {
    return (i->points)>(j->points);
}

int ITEMless(Item i, Item j) {
    return (i->points)<(j->points);
}

void ITEMdisplay(FILE* fp, Item i, const char *mode) {
    if (mode[0]=='o') {
        fprintf(fp, "%15s: %d punt%c\n", i->name, i->points, i->points==1?'o':'i');
    } else if (mode[0]=='f'){
        fprintf(fp, "%s %d\n", i->name, i->points);
    }
}
