#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOME_FILE_IN "testo.txt"
#define NOME_FILE_OUT "output.txt"
#define MAX_SCELTE 10

typedef struct {
    char scelte[MAX_SCELTE];
    int num_scelte;
} Livello;


int seq(int pos, Livello *val, char *sol, int n, int count, FILE *fp) {
    int i;
    if (pos>=n) {
        for (i = 0; i < n; i++) fprintf(fp, "%c", sol[i]);
        fprintf(fp, "\n");
        return count+1;
    }

    for (i = 0; i < val[pos].num_scelte; i++) {
        sol[pos] = val[pos].scelte[i];
        count = seq(pos+1, val, sol, n, count, fp);
    }

    return count;
}

int main() {

    FILE *fp;
    int n=0, i;
    Livello *val;
    char *sol;

    if ((fp=fopen(NOME_FILE_IN, "r"))==NULL) exit(1);
    if (fscanf(fp, "%d", &n)!=1) exit(2);

    // alloco il vettore con tutti i livelli
    val=(Livello*)malloc(n*sizeof(Livello));
    sol=(char*)malloc(n*sizeof(char));

    for (i=0; i<n; i++) {
        fscanf(fp, "%s", val[i].scelte);
        val[i].num_scelte=strlen(val[i].scelte);
    }
    fclose(fp);

    // uso fp per aprire il file di outpu
    if ((fp=fopen(NOME_FILE_OUT, "w"))==NULL) exit(1);

    seq(0, val, sol, n, 0, fp);
    fclose(fp);

    return 0;
}

