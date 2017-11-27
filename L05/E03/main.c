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


void seq(int pos, Livello *val, char *sol, int n, FILE *fp) {
    int i;
    // caso terminale la soluzione ha n elementi
    if (pos>=n) {
        for (i = 0; i < n; i++) fprintf(fp, "%c", sol[i]);
        fprintf(fp, "\n");
        return;
    }

    for (i = 0; i < val[pos].num_scelte; i++) {
        sol[pos] = val[pos].scelte[i];
        seq(pos+1, val, sol, n, fp);
    }

    return;
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
        // prendo tutta la stringa e la salvo
        fscanf(fp, "%s", val[i].scelte);
        // ovviamente il numero di scelete possibili sull'i-esimo livello
        // è il numero di caratteri che ci sono dentro val[i].scelta
        val[i].num_scelte=strlen(val[i].scelte);
    }
    fclose(fp);

    // riciclo fp per aprire il file di output
    if ((fp=fopen(NOME_FILE_OUT, "w"))==NULL) exit(1);

    // lancio la ricorsione che scriverà tutte le soluzioni su fp
    seq(0, val, sol, n,  fp);
    fclose(fp);

    return 0;
}

