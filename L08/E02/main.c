#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define FILE_FRECCE "frecce.txt"

int comb(int*, int, int, int*);
int getCosto(int*, int*, int);
int isBalanced(int, int*, int);

int main() {
        
    FILE *fp;
    int i,n;
    int *f, *sol;

    if ((fp=fopen(FILE_FRECCE, "r"))==NULL){
        printf("Errore! Impossibile aprire il file \"%s\"\n", FILE_FRECCE);
        exit(1);
    }
    
    // lettura della prima riga
    fscanf(fp, "%d", &n);
    
    // lettura dell numereo delle frecce
    f=(int*)malloc(n*sizeof(int));
    sol=(int*)malloc(n*sizeof(int));
    
    
    // lettura delle freccie
    for (i=0; i<n; i++) {
        fscanf( fp, "%d", &f[i]);
    }
    
    comb(f,n,0,sol);

    return 0;
}

int comb(int *f, int n, int pos, int *sol) {

    static int costoMin=INT_MAX;
    
    int i,newCosto;
    // condizione di terminazione
    if (pos==n) {
        newCosto=getCosto(f,sol,n);
        if (newCosto<costoMin && isBalanced(0, sol, n)) {
            costoMin=newCosto;
            for (i=0; i<pos; i++) printf("%c", sol[i]==0?'>':'<');
            printf("\nCosto: %d\n", costoMin);
        }
        return costoMin;
    }
    
    sol[pos]=0;
    costoMin=comb(f,n,pos+1,sol);
    sol[pos]=1;
    costoMin=comb(f,n,pos+1,sol);
    return costoMin;
}

int getCosto(int *f, int*sol, int n) {
    int i, costoTot=0;

    for (i=0; i<n; i++) {
        // se devo girare la freccia aumento il costo totale
        if (f[i]!=sol[i]) costoTot++;
    }
    return costoTot;
}

int isBalanced(int l, int *sol, int n) {
    
    // condizione di terminazione
    if (l>=n) return 1;
    
    int i=l;
    int currArrow=sol[l];
    int leftBalance=1;
    int rightBalance=1;
       
    // se la prima freccia a sinistra punta verso destra vuol dire che 
    // c'è qualcosa che non va e quindi la sequenza non è bilanciata
    if (currArrow==1) return 0;
    
    while (currArrow==sol[++i]) leftBalance++;
    currArrow=sol[i]; // siamo sulla prima freccia nell'altra direzione
    while (currArrow==sol[++i]) rightBalance++;
    
    // controllo e ricorsione
    if (rightBalance!=leftBalance) return 0;
    
    return isBalanced(i, sol, n);
}