#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define FILE_FRECCE "frecce.txt"

#define inf INT_MAX

int comb(int*, int, int, int*, int*);
int din(int*, int, int, int, int*, int*);
int getCosto(int*, int*, int);
int getCostoBanale(int*, int, int);
int isBalanced(int, int*, int);

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
    sol=(int*)malloc(n*sizeof(int));
    bestSol=(int*)malloc(n*sizeof(int));
    
    // lettura delle frecce
    for (i=0; i<n; i++) {
        fscanf(fp, "%d", &f[i]);
    }
    fclose(fp);
    
    puts("Metodo programmazione dinamica...");
    
    i=din(f,0,n-1,0,sol,bestSol);
    printf("Costo: %d\n", i);
    
    for (i=80; i-->0; printf("-"));
    /*
    printf("Comincio metodo calcolo combinatorio con %d frecce\n", n );
    comb(f,n,0,sol,bestSol);
    printArrows(bestSol, n);
    printf("Costo: %d\n", getCosto(f, bestSol, n));
    */
    return 0;
}

int din(int *f, int l, int r, int pos, int *sol, int *bestSol) {
    
    int i, newMin;
    int bestMin=INT_MAX;
    
    // condizione di terminazione
    if (l>=r) return 0;
    
    if ((r-l)==1) {
        //printf("ciao\n");
        return getCostoBanale(f, l, r);
    }
        
    // in tutti gli altri casi ricorro
    for (i=l+2; i<=r; i+=2) {
        newMin=getCostoBanale(f, l, i-1)+din(f,i,r,0,sol,bestSol);
        printf("l=%d, r=%d, newMin=%d, bestMin=%d\n", i, r, newMin, bestMin);
        if (newMin<bestMin) bestMin=newMin;
    }
    return bestMin;
}

int getCostoBanale(int *f, int l, int r) {
    // calcola il costo per portare le frecce nella configurazione
    // ><   >><<    >>><<<      etc
    if (l>=r) return 0;
    
    int i, cntr=0;
    int m=(r+l)/2;
    for (i=l; i<=r; i++) {
        if      (i> m && f[i]!=1) cntr++;
        else if (i<=m && f[i]!=0) cntr++;
    }
    return cntr;
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

int getCosto(int *f, int*sol, int n) {
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
    int leftBalance=1;
    int rightBalance=1;
       
    // se la prima freccia a sinistra punta verso destra vuol dire che 
    // c'è qualcosa che non va e quindi la sequenza non è bilanciata
    if (currArrow==1) return 0;
    
    while (currArrow==sol[++i]) leftBalance++; // mi sposto alla prima freccia
    currArrow=sol[i]; // siamo sulla prima freccia nell'altra direzione
    while (currArrow==sol[++i]) rightBalance++;
    
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