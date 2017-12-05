#include <stdio.h>
#include <stdlib.h>

#define FILE_ABITANTI "hab.txt"
#define FILE_FAMIGLIE "fam.txt"
#define FILE_NEMICI "nem.txt"

#define MAX_NEMICI 100

typedef struct {
    int *koalaIndex;
    int nKoalaIndex;
    int famNemicaIndex[MAX_NEMICI];
    int nfamNemicheIndex;
} Famiglia;

typedef struct {
    int famIndex;
    int *alberi;
    int nAlberi;
} Koala;

typedef struct {
    Koala *koala;
    int nKoala;
    Famiglia *famiglie;
    int nFamiglie;    
    int nAlberiTot;
    int maxPerAlbero;
} Popolazione;

FILE *apriFile(const char*, const char*);
int alberoExists(int, Popolazione*, int);

int areEnemies(int a, int b, Popolazione *p) {
    // funzione che dati due Indici di koala a e b controlla se sono nemici
    int i,famA, famB;
    
    famA=p->koala[a].famIndex;
    famB=p->koala[b].famIndex;
    
    for (i=0; i<p->famiglie[famA].nKoalaIndex; i++) {
        if (p->famiglie[famA].famNemicaIndex[i] == famB)
            return 1;
    }
    return 0;
}

int isValid(int n, int *sol, int m, Popolazione *p) {
    int i,j,k;
    int totAlbero;
    
    int *solKoala=(int*)malloc(n*sizeof(int));
    
    // per ogni albero
    for (i=0; i<m; i++) {
        totAlbero=0;
        // estrazione dei koala su un albero
        for (j=0; j<n; j++) {
            if (sol[j]==i) { // se fa parte del gruppo che sto considerando
                solKoala[totAlbero++]=j;
            }
        }
        // controllo quantità di koala
       // printf("controllo albero... ");
        if (totAlbero > p->maxPerAlbero) return 0;
       // printf("passato!\n");
        
       // printf("controllo famiglie ...");
        // controllo incopatibilità per famiglie
        for (j=0; j<totAlbero; j++) { //j indice della solKoala
            for (k=j; k<totAlbero; k++) {
                if (areEnemies(j,k, p)) return 0;
            }
        }
       // printf("passato!\n");
        
    }
    return 1;
}

// algoritmo di er
int er(int n, int m, int pos, int *sol, Popolazione *p) {
    static int count=0;
    static int continua=1;
    
    if (!continua) return count;
    
    int i,j;
    
    if (pos>=n) {
        count++;
        if (isValid(n,sol,m,p)) {
            
            printf("partizione in %d blocchi: ", m);
            for (i=0; i<m; i++) {
                printf("{ ");
                for (j=0; j<n; j++)
                    if (sol[j]==i)
                        printf("%d " ,j);
                printf("} ");
            }
            printf("\n%d operazioni.\n", count);
            continua=0;
        }
        return count;
    }
    
    for (i=0; i<m; i++) {
        sol[pos]=i;
        er(n,m,pos+1,sol,p);
    }
return count; 
}

int main(int argc, char **argv){
    
    int i, j, n, x;
    int *sol;
    Popolazione p;
    FILE *fp;
    
    p.nAlberiTot=0; // azzero la variabile contatore alberi
    
    if (argc!=2) {
        puts("Errore! Inserire il numero corretto di parametri!");
        exit(1);
    }
    p.maxPerAlbero=atoi(argv[1]);
    
    // lettura file degli abitanti
    fp=apriFile(FILE_ABITANTI, "r");
    
    // leggo il numero totale di koala dalla prima riga del file
    fscanf(fp, "%d", &p.nKoala);
    p.koala=(Koala*)malloc(p.nKoala*sizeof(Koala));
    
    for (i=0; i<p.nKoala; i++) {
        // per ogni koala che trovo alloco il vettore corretto di alberi
        // su cui può vivere, parcheggio il numero di alberi temporaneo in n
        fscanf(fp, "%d", &p.koala[i].nAlberi);
        p.koala[i].alberi=(int*)malloc(n*sizeof(int));
        
        for (j=0; j<p.koala[i].nAlberi; j++) {
            fscanf(fp, "%d", &x);
            p.koala[i].alberi[j]=x;
            
            // controllo e eventuale aggiornamento degli alberi totali
            if (!alberoExists(x, &p, i)) p.nAlberiTot++;
                       
        }
    }
    fclose(fp);
        
    // leggo il file delle famiglie
    fp=apriFile(FILE_FAMIGLIE, "r");
    fscanf(fp, "%d", &p.nFamiglie);
    // alloco vettore di dimensione opportuna
    p.famiglie=(Famiglia*)malloc(p.nFamiglie*sizeof(Famiglia));
    
    for (i=0; i<p.nFamiglie; i++) {
        // assegno ad ogni famiglia i suoi koala e ad ogni koala la sua famiglia
        // dove n è il numero di membri di ciascuna famiglia
        // già che ci sono inizializzo il valore delle famiglie nemiche
        p.famiglie[i].nfamNemicheIndex=0;
        
        fscanf(fp, "%d", &p.famiglie[i].nKoalaIndex);
        p.famiglie[i].koalaIndex=(int*)malloc(n*sizeof(int));
        for (j=0; j<p.famiglie[i].nKoalaIndex; j++) {
            fscanf(fp, "%d", &x);
            p.famiglie[i].koalaIndex[j]=x;
            // assegnazione della famiglia al koala x 
            p.koala[x].famIndex=i;
        }
    }
    fclose(fp);
    
    // leggo il file delle fmaiglie nemiche
    fp=apriFile(FILE_NEMICI, "r");
    // riuso le variabili x e n 
    i=0;
    while (fscanf(fp, "%d %d" , &x, &n)==2 && i<MAX_NEMICI) {
        // inserisco le famiglie nemiche reciprocamente
        p.famiglie[x].famNemicaIndex[p.famiglie[x].nfamNemicheIndex++]=n;
        p.famiglie[n].famNemicaIndex[p.famiglie[n].nfamNemicheIndex++]=x;
        i++;
    }
    fclose(fp);
    // fine della lettura dei dati
    
    // ricerca della soluzione per ogni valore di alberi nella foresta
    sol=(int*)malloc(n*sizeof(int));
    for (i=1; i<=p.nAlberiTot; i++)
        er(p.nKoala, i, 0, sol, &p);
    

    getc(stdin);
    
    return 0;
}



int alberoExists(int x, Popolazione *p, int n) {
    int i,j;
    // se è il primo koala accetta tutti gli alberi nuovi
    if (n==0) return 0;
    
    // controllo per ogni koala precente a quello che si sta considerando
    // se esiste già l'lbero che si vorrebbe aggiungere al conteggio
    for (j=0; j<=n-1; j++)
        for (i=0; i<p->koala[j].nAlberi; i++)
            if (p->koala[j].alberi[i]==x)
                return 1;
    return 0;
}

FILE *apriFile(const char *nome, const char *modo) {
    FILE *fp;
    if ((fp=fopen(nome, modo))==NULL) {
        printf("Impossibile aprire il file \"%s\"\n", FILE_ABITANTI);
        getc(stdin);
        exit(1);
    }
    return fp;
}