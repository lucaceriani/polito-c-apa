#include <stdio.h>
#include <stdlib.h>

#define NOME_FILE "testo.txt"
#define MAXS 10
#define N 2

#define non_strutturato ;;

typedef struct item_s {
    int a, b;
    char c;
    float d;
    char s[MAXS];

} Item;

int main() {

    int i,j;
    FILE *fp;
    Item M[N][N];

    // apro e controllo il file
    if ((fp=fopen(NOME_FILE, "r"))==NULL) exit(1);

    for (i=0; i<N; i++)
        for (j=0; j<N; j++)
            fscanf(fp, "%d%d %c%f%s", &M[i][j].a, &M[i][j].b, &M[i][j].c,
                   &M[i][j].d, M[i][j].s);

    for(non_strutturato) {
        printf("[r, c] > ");
        if (scanf("%d%d", &i, &j)!=2) exit(1);
        printf("Contetnuto %d, %d\n", i, j);
        printf("%d %d %c %f %s\n", M[i][j].a, M[i][j].b, M[i][j].c, M[i][j].d,
               M[i][j].s);
        printf("%p\n%p\n%p\n%p\n%p\n", &M[i][j].a, &M[i][j].b, &M[i][j].c,
               &M[i][j].d, M[i][j].s);
    }

    return 0;
}
