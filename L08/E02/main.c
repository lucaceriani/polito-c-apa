#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define FILE_FRECCE "frecce2.txt"
#define inf INT_MAX

int comb(int*, int, int, int*, int*);
int din(int*, int, int, int, int*, int*);
int getCosto(int*, int*, int);
int isBalanced(int, int*, int);
void equalizza(int*, int, int);
void printArrows(int*, int);

int main() {
        
    FILE *fp;
    int i,n;
    int *f, *sol, *bestSol;

    if ((fp=fopen(FILE_FRECCE, "r"))==NULL){
        printf("Errore! Impossibile aprire il file \"%s\"\n", FILE_FRECCE);
        exit(1);
    }
    printf("Sto usando il file \"%s\"...\n", FILE_FRECCE);
    
    // lettura della prima riga
    fscanf(fp, "%d", &n);
    printf("Ho trovato %d frecce...\n", n);
    
    
    // lettura dell numereo delle frecce
    f=(int*)malloc(n*sizeof(int));
    sol=(int*)calloc(n,sizeof(int));
    bestSol=(int*)calloc(n,sizeof(int));
    
    // lettura delle frecce
    for (i=0; i<n; i++) {
        fscanf(fp, "%d", &f[i]);
    }
    fclose(fp);
    printArrows(f, n);
    equalizza(sol, 0, n-1);
    equalizza(bestSol, 0, n-1);

    puts("Metodo programmazione dinamica...");
    din(f,0,n-1,0,sol,bestSol);
    printArrows(bestSol, n);
    //i=Gira(n,f);
    printf("Costo: %d\n", getCosto(f, bestSol, n));
    for (i=80; i-->0; printf("-"));

    equalizza(sol, 0, n-1);
    equalizza(bestSol, 0, n-1);
    
    puts("Metodo calcolo combinatorio...");
    comb(f,n,0,sol,bestSol);
    printArrows(bestSol, n);
    printf("Costo: %d\n", getCosto(f, bestSol, n));
    
    return 0;
}

void equalizza(int *sol, int l, int r) {
    int i;
    int mid=(l+r)/2;
    for (i=l; i<=r; i++) {
        if   (i<=mid) sol[i]=0;
        else sol[i]=1;
    }
}

void autoUpdateSolution(int *f, int* sol, int *bestSol, int n) {
    if (getCosto(f, sol, n)<getCosto(f, bestSol, n) && isBalanced(0, sol, n)){
        memcpy(bestSol, sol, n*sizeof(int)); // aggiona soluzione
    }
}

int din(int *f, int l, int r, int pos, int *sol, int *bestSol) {
    // r è l'indice dell'ultima casella, non il numero di elementi
    
    int i;
    int n=r+1;
    
    // condizione di terminazione
    if (l>=r) return 0;
    // se ho solo una cella
    if ((r-l)==1){
        equalizza(sol, l, r);
        autoUpdateSolution(f, sol, bestSol, n);
        return 0;
    }
        
    // in tutti gli altri casi ricorro    
    for (i=l; i<r+2; i+=2) {
        equalizza(sol, l, i+1);
        din(f,i+2,r,0,sol,bestSol);
        autoUpdateSolution(f, sol, bestSol, n);
    }
    return getCosto(f, bestSol, n);
}


int comb(int *f, int n, int pos, int *sol, int *bestSol) {

    static int costoMin=INT_MAX;
    
    int newCosto;
    // condizione di terminazione
    if (pos==n) {
        newCosto=getCosto(f,sol,n);
        if (newCosto<costoMin && isBalanced(0, sol, n)) {
            costoMin=newCosto; // aggiorno il costo minimo
            memcpy(bestSol, sol, n*sizeof(int)); // mi salvo la soulzione
        }
        return costoMin;
    }
    
    sol[pos]=0;
    costoMin=comb(f,n,pos+1,sol,bestSol);
    sol[pos]=1;
    costoMin=comb(f,n,pos+1,sol,bestSol);
    
    return costoMin;
}

int getCosto(int *f, int *sol, int n) {
    // calcolo del costo della soluzione rispetto a f (vettore contente)
    // le frecce lette da file
    int i, costoTot=0;

    for (i=0; i<n; i++) {
        // se devo girare la freccia aumento il costo totale
        if (f[i]!=sol[i]) costoTot++;
    }
    return costoTot;
}

int isBalanced(int l, int *sol, int n) {
    
    // condizione di terminazione: se sono arrivato ad un vettore unitario
    //                             oppure se ho sforato
    if (l>=n) return 1;
    
    int i=l;
    int currArrow=sol[l];
    int leftBalance=0;
    int rightBalance=0;
       
    // se la prima freccia a sinistra punta verso destra vuol dire che 
    // c'è qualcosa che non va e quindi la sequenza non è bilanciata
    if (currArrow==1) return 0;
    
    while (currArrow==sol[++i] && i<n) leftBalance++; // mi sposto alla prima freccia
    currArrow=sol[i]; // siamo sulla prima freccia nell'altra direzione
    while (currArrow==sol[++i] && i<n) rightBalance++;
    
    // controllo e ricorsione
    if (rightBalance!=leftBalance) return 0;
    
    return isBalanced(i, sol, n);
}

void printArrows(int* sol, int n) {
    // stampa le frecce dentro sol
    int i;
    for (i=0; i<n; i++)
        printf("%c", sol[i]==0?'>':'<');
    printf("\n");
}