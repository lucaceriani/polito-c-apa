#include <stdio.h>
#include <stdlib.h>

#define DBG 1

#define FILE_RISORSE "res.txt"
#define FILE_MAPPA "map.txt"

#define NORM(x) (((x)<0)?(0):(x))

typedef struct {
    int r, att, dif, cost;
} Risorsa;

typedef struct {
    int ami; // 1=amico, 0=nemico
    int val;
} Target;

int risolvi(Target**, int**, int**, int, int, Risorsa*, int, int, int, int, int);
int copreQualcosa(Target**, int**, int, int, int, int);
int check(Target**, int**, int**, int, int, Risorsa*);
void showMap(Target**, int**, int, int);

int main() {
    
    FILE *fp;
    Risorsa *res;
    int **resMap;
    int **inGame;
    Target **tarMap;
    int nRes, i, r, c, budget, j, k, t;
    // lettura delle risorse e allocazione della struttura dati
    if((fp=fopen(FILE_RISORSE, "r"))==NULL) exit(EXIT_FAILURE);
    fscanf(fp, "%d", &nRes);
    nRes++; // aggiungo anche la risorsa nulla
    res=(Risorsa*)malloc(nRes*sizeof(Risorsa));
    res[0].r=0; res[0].att=0; res[0].dif=0; res[0].cost=0; // risorsa nulla
    for (i=1; i<nRes; i++)
        fscanf(fp, "%d %d %d %d", &res[i].r, &res[i].att, &res[i].dif, &res[i].cost);
    fclose(fp);
    printf("Lette %d risorse dal file \"%s\"...\n", nRes-1, FILE_RISORSE);
    
    // lettura della mappa e allocazione matrice
    if((fp=fopen(FILE_MAPPA, "r"))==NULL) exit(EXIT_FAILURE);
    fscanf(fp, "%d %d", &r, &c);
    tarMap=(Target**)malloc(r*sizeof(Target*));
    for (i=0; i<r; i++){
        tarMap[i]=(Target*)malloc(c*sizeof(Target));
    }
    
    // azzero la memoria
    for (i=0; i<r; i++){
        for (j=0; j<c; j++) {
            tarMap[i][j].ami=-1;
            tarMap[i][j].val=-1;
        }
    }
    
    while (fscanf(fp, "%d %d %d %d", &t, &i, &j, &k)==4) {
        tarMap[i][j].ami=t;
        tarMap[i][j].val=k;
    }
    fclose(fp);
    printf("Letta matrice %dx%d dal file \"%s\"...\n", r, c, FILE_MAPPA);
   
    // allocazione della mappa dove andranno inserite le risorse (indice di res)
    resMap=(int**)malloc(r*sizeof(int*));
    for (i=0; i<r; i++)
        resMap[i]=(int*)calloc(c,sizeof(int));
        
    // mappa per tenere conto di quelli uccisi/salvati
    inGame=(int**)malloc(r*sizeof(int*));
    for (i=0; i<r; i++)
        inGame[i]=(int*)calloc(c,sizeof(int));
        
    showMap(tarMap, resMap, r, c);
    
    printf("Inserire un budget: ");
    scanf("%d", &budget);
    //budget=60;
    printf("Inizio risoluzione del problema...\n");
    
    risolvi(tarMap, resMap, inGame, r, c, res, nRes, budget, 0, 0, 0);    
    return 0;
}

int risolvi(Target **tarMap, int **resMap, int **inGame, int r, int c,
            Risorsa *res, int nRes, int budget, int start, int prezzo, int pos) {
    int i;
    // condizione di terminazione: ho riempito tutte le righe della matrice
    if (pos==r*c) {
#if DBG == 1
            if (check(tarMap, resMap, inGame, r, c, res)) {
                showMap(tarMap, resMap, r, c);
                printf("prezzo: %d\n", prezzo);
                system("pause>nul");
                system("cls");
            }
#endif            
            return 0;
    }

    for (i=start; i<nRes; i++) {
        
        resMap[pos/c][pos%c]=i; // converto la posizione nelle coordinate della matrice
        
        // pruning
        if (tarMap[pos/c][pos%c].ami!=-1) resMap[pos/c][pos%c]=0;
        if (prezzo+res[i].cost>budget) continue; // non considero quello che costa troppo
        if (i!=0 && !copreQualcosa(tarMap, resMap, r, c, pos, res[i].r)) continue;
        
        // ricorsione
        risolvi(tarMap, resMap, inGame, r, c, res, nRes, budget, start, prezzo+res[i].cost, pos+1);
        //start++;
    }
        
    return 0;
}

int check(Target **tarMap, int **resMap, int **inGame, int r, int c, Risorsa *res) {
    // check dei nemici
    int i,j, k, h, rad;
    
    for (i=0; i<r; i++)
        for (j=0; j<c; j++)
            inGame[i][j]=0;
    
    // posizione in alto a sinistra dell'area di attacco della struttura
    // NORM rende i valori minori di zero uguali a zero
    
    // applicazione degli effetti sui nemici
    for (i=0; i<r; i++)
        for (j=0; j<c; j++)
            if ((rad=res[resMap[i][j]].r)!=0)
                for (k=NORM(i-rad); k<2*rad+NORM(i-rad) && k<r; k++)
                    for (h=NORM(j-rad); h<2*rad+NORM(j-rad) && h<c; h++)
                        inGame[k][h]+=res[resMap[i][j]].att;
    
    for (i=0; i<r; i++)
        for (j=0; j<c; j++)
            if (tarMap[i][j].ami==0 && tarMap[i][j].val-inGame[i][j]>0)
                return 0;
    
    // reset matrice
    for (i=0; i<r; i++)
        for (j=0; j<c; j++)
            inGame[i][j]=0;
    
    // applicazione effetti sugli alleati
   for (i=0; i<r; i++)
        for (j=0; j<c; j++)
            if ((rad=res[resMap[i][j]].r)!=0)
                for (k=NORM(i-rad); k<2*rad+NORM(i-rad) && k<r; k++)
                    for (h=NORM(j-rad); h<2*rad+NORM(j-rad) && h<c; h++)
                        inGame[k][h]+=res[resMap[i][j]].dif;
        
    // conteggio per gli alleati
    for (i=0; i<r; i++)
        for (j=0; j<c; j++)
            if (tarMap[i][j].ami==1 && tarMap[i][j].val-inGame[i][j]>0)
                return 0;

    return 1;
}

int copreQualcosa(Target **tarMap, int **resMap, int r, int c, int pos, int rad) {
    int i,j;
    int posR, posC; // upper left corner
    
    // posizione del centro della struttura
    posR=pos/c;
    posC=pos%c;
    
    // posizione in alto a sinistra dell'area di attacco della struttura
    // NORM rende i valori minori di zero uguali a zero
    posR=NORM(posR-rad);
    posC=NORM(posC-rad);
    
    
    for (i=posR; i<2*rad+posR && i<r; i++)
        for (j=posC; j<2*rad+posC && i<c; j++)
            if (tarMap[i][j].ami!=-1) return 1;
    return 0;
    
}

void showMap(Target **tarMap, int **resMap, int r, int c) {
    int i,j;
    printf("  ");
    for (j=0; j<c; j++) printf("  %d ", j); printf("\n");
    for (i=0; i<r; i++) {
        
        for (j=0, printf("  +"); j<c; j++) printf("---+"); printf("\n");
        printf("%d ", i);
        for (j=0; j<c; j++) {
            printf("| ");
            if (tarMap[i][j].ami==1) printf("A");
            else if (tarMap[i][j].ami==0) printf("E");
            else if (resMap[i][j]!=0) printf("%d", resMap[i][j]);
            else printf(" ");
            printf(" ");
        }
        printf("|\n");
    }
    for (j=0, printf("  +"); j<c; j++) printf("---+"); printf("\n");
}