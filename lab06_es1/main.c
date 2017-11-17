#include <stdio.h>
#include <stdlib.h>

#define NOME_FILE "prodotti.txt"
#define MAX_NOME 25

typedef struct {
    char prodotto[MAX_NOME];
    float prezzo;
} Item;

float sum_price(Item* p, int n) {
    float sum=0;
    int i;
    for (i=0;i<n;i++) {
        sum=sum+p[i].prezzo;
    }
    return sum;
}

int carrello(Item* p, int n, int k, float b, Item *sol, int n_sol, int start, int count) {
    int i;
    float sum=sum_price(sol, n_sol);
    if (sum <= b && n_sol>=k) {
        for (i=0; i<n_sol; i++) {
            printf("%s ", sol[i].prodotto);
        }
        printf(".................%.2f\n", sum);
        return count+1;
    }

    // altrimenti scelgo il primo prodotto non ancora scelto e ricorro
    for (i=start; i<n; i++) {
        sol[n_sol]=p[i];
        count=carrello(p, n, k, b, sol, n_sol+1, i+1, count);
    }

    return count;
}

int main() {
    int i, n, k, c=0, soluzioni=0;;
    float b;
    FILE *fp;
    Item *p, *sol;


    // apro il file
    if ((fp=fopen(NOME_FILE, "r"))==NULL) exit(1);

    // controllo la prima riga
    if (fscanf(fp, "%d", &n)!=1) exit(1);

    // alloco la memoria necessaria
    p=(Item*)malloc(n*sizeof(Item));

    // alloco il vettore delle soluzioni max n elementi, min k
    sol=(Item*)calloc(n, sizeof(Item));

    // inserimento dei valori in p
    for (i=0; i<n; i++) {
        fscanf(fp, "%s %f", p[i].prodotto, &p[i].prezzo);
    }
    fclose(fp);

    printf("Numero minimo di elementi: ");
    scanf("%d", &k);
    printf("Budget massimo: ");
    scanf("%f", &b);

    for(;;) {
        // provo dal k inserito fino a quando non trovo più soluzioni
        // ad ogni passo incremento k di 1.
        c=carrello(p, n, k++, b, sol, 0, 0, 0);
        if (c==0) break;
        else soluzioni+=c;
    }

    printf("\n\n%d soluzion%c trovat%c!", soluzioni, soluzioni==1?'e':'i', soluzioni==1?'a':'e');

}


