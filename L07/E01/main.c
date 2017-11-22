#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NOME_CITTA 20

typedef struct {
    char nome[MAX_NOME_CITTA+1];
    int pop; // popolazione
    int distAss; // distanza assoluta dall'inizio della via
    int *distRel; // vettore delle distanze relative
} Item;

void cercaOttima(Item*, int, Item*, int, int, int, Item*);
void check(Item*, int, Item*, int, Item*);
int sommaDistanze(Item*, int, Item*, int);

int main(int argc, char **argv) {

    int k, n, i, j;
    Item *citta, *sol, *solOttima;
    FILE *fp;

    // i parametri devono essere <nomefile> <k>
    if (argc!=3) puts("Errore, inserire tutti i parametri!"), exit(1);

    // converto il valore di k a intero
    k=atoi(argv[2]);

    // apro il file
    if ((fp=fopen(argv[1], "r"))==NULL){
        printf("Errore nell'apertura del file \"%s\"!\n", argv[1]);
        exit(1);
    }

    // leggo la prima riga del file e la controllo
    if (fscanf(fp, "%d\n", &n)!=1) puts("Errore nei dati!"), exit(1);

    // errore se k>=n
    if (k>=n) puts("Errore, k deve essere < delle citta'!"), exit(1);

    // alloco il vettore di strutture e delle soluzioni
    citta=(Item*)malloc(n*sizeof(Item));
    sol=(Item*)malloc(k*sizeof(Item));
    solOttima=(Item*)malloc(k*sizeof(Item));

    // leggo le n righe successive
    for (i=0; i<n; i++) {
        fscanf(fp, "%s %d %d", citta[i].nome, &citta[i].pop, &citta[i].distAss);
    }
    fclose(fp);

    for (i=0; i<n; i++) {
        // alloco il vettore distanza relativa per ogni struct
        citta[i].distRel=(int*)malloc(n*sizeof(int));
        // calcolo delle distanze relative
        // N.B. citta[i].distRel[j] conterrà la distanza relativa tra
        //      la città i e la città j
        for (j=0; j<n; j++) {
            citta[i].distRel[j]=abs(citta[i].distAss-citta[j].distAss);
        }
    }


    // Impongo una soluzione ottima fittizia per non leggere in zone
    // di memoria non ancora inizializzate a Item
    for (i=0; i<k; i++) solOttima[i]=citta[i];

    cercaOttima(citta, n, sol, 0, k, 0, solOttima);

    puts("La soluzione ottima e':\n");
    for (i=0; i<k; i++) puts(solOttima[i].nome);

    return 0;
}

void cercaOttima(Item* citta, int n, Item* sol, int nSol, int k, int start,
    Item *solOttima) {

    int i;
    // condizione di terminazione
    if (nSol==k) {
        // controllo e eventuiale aggiornamento della solOttima
        check(citta, n, sol, nSol, solOttima);
        return;
    }

    for (i=start; i<n; i++) {
        // scelgo la soluzione corrente
        sol[nSol]=citta[i];
        // ricorro sulla città dopo e con una soluzione in più
        cercaOttima(citta, n, sol, nSol+1, k, i+1, solOttima);
    }
}

void check(Item* citta, int n, Item* sol, int nSol, Item* solOttima) {
    int i;
    // s'intende che nSol è uguale al numero di soluzioni in solOttima che
    // a sua volta è uguale a k

    // se la soluzione attuale minimizza meglio la precedente soluzione ottima
    // allora questa diventa la soluzione ottima
    if (sommaDistanze(citta,n,sol,nSol)<sommaDistanze(citta,n,solOttima,nSol)) {
        for (i=0; i<nSol; i++)
            solOttima[i]=sol[i];
    }
}

int sommaDistanze(Item *citta, int n, Item *sol, int nSol) {
    int i, j, somma=0, distMinDaATO;

    // for che cicla su ogni città e ne calcola la somma delle distanze
    for (i=0; i<n; i++) {
        distMinDaATO=INT_MAX;

        // cerco la distanza minima da ATO iterando la distanza della i-esima
        // città sulla j-esima città della soluzione
        // (distanza a->b == distanza b->a)
        for (j=0; j<nSol; j++) {
            if (sol[j].distRel[i]<distMinDaATO) {
                    distMinDaATO=sol[j].distRel[i];
            }
        }
        somma+=citta[i].pop*distMinDaATO;
    }
    return somma;
}
