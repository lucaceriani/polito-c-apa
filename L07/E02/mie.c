#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
    #define F_CLEAR "cls"
#else
    #define F_CLEAR "clear"
#endif // _WIN32

int _comp(char *a, char *b){
    return  strcasecmp(a,b);
}

void strcatMia(char *dest, char *src1, char *src2) {
    strcpy(dest, src1);
    strcat(dest, src2);
}

int startsWith(char *a, char *b) {
    // case unsensitive
    int i, n=0;
    // voglio proseguire il confronto fino all'ultima lettera
    // di b (che dorevbbe essere la più corta)

    n=strlen(b);
    if (strlen(a)<n) return 0;

    for (i=0; i<n; i++)
        if (tolower(a[i])!=tolower(b[i]))
            return 0;

    return 1;
}

void cls() {
    system(F_CLEAR);
}
