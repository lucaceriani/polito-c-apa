#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_ELEMENTI "elementi.txt"
#define MAX_ELEMENTI 8

enum cat {
    NA, AA, AI
};

typedef struct {
    enum cat categoria;
    char *nome;
    int grado;
    float punti;
} Elemento;

unsigned int cnt=0;

float calcolaPunteggio(Elemento *e, int tot, int *sol, int n, int niceOutput) {
    // calcolo del punteggio tenendo conto di tutti i vincoli
    int i;
    float punteggio=0;
    int categorie[3]={0};
    int *presi=calloc(tot, sizeof(int));
    int nCat=0;
    int diff=0;
    
    for (i=0; i<n; i++) { // per ogni elemento
        diff+=e[sol[i]].grado;
        categorie[e[sol[i]].categoria]++; // contatore della cat corrisp
        
        if (!presi[sol[i]]) { // se non è mai stato preso lo prendo
            punteggio+=e[sol[i]].punti;
            presi[sol[i]]=1;
        }
    }
    
    // controllo il bonus di uscita
    if (e[sol[n-1]].categoria!=NA && e[sol[n-1]].grado>=5){
        if (niceOutput) printf("Uscita: +2.50\n");
        punteggio+=2.5;
        // se l'ultimo elemento ha già partecipato al bonus di uscita non potrà
        // partecipare la bonus della composizione, quindi tolgo la sua
        // categoria dal contatore delle categorie
        categorie[e[sol[n-1]].categoria]--;
    }
    
    // possibile bonus di composizione
    for (i=0; i<3; i++) if (categorie[i]) nCat++;
    if (nCat>1){ // se sono state presentate più di una categoria -> bonus
        if (niceOutput) printf("Composizione: +2.50\n");
        punteggio+=2.5;
    }
    
    free(presi); // libero il vettore
    return punteggio;
}

int pruValid(Elemento e, char **argv) {
    // controllo che l'elemento no sia troppo difficile per l'altleta
    // dato che la categori è un enum e sono in ordine come i parametri posso
    // scrivere così
    
    if (e.grado > atoi(argv[1+e.categoria])) return 0; // se è troppo difficile
    // else
    return 1;
}

void displaySol(Elemento* e, int *sol, int n) {
    int i;
    for (i=0; i<n; i++) {
        printf("%s %d %+.2f\n", e[sol[i]].nome, e[sol[i]].grado, e[sol[i]].punti);
        
    }
}

void aggiornaSol(int *dst, int *src, int n) {
    int i;
    for (i=0; i<n; i++) dst[i]=src[i];
}

float calcolo(Elemento *e, int *sol, int *bestSol, int tot, int nSol,
            int start, int diff, float bestPunti, int maxDiff, char **argv) {

    int i;
    float punti;
    
    if (++cnt%1000000==0) printf("%d milioni\n", cnt/1000000); // contatore
    
    // condizione di teminazione
    if (nSol==MAX_ELEMENTI) {
        punti=calcolaPunteggio(e, tot, sol, nSol, 0);
        if (punti>bestPunti) {
            aggiornaSol(bestSol, sol, nSol);
            bestPunti=punti;
        }
        return bestPunti;
    }
    
    for (i=start; i<tot; i++) {
        
        // pruning 
        if (diff+e[i].grado>maxDiff){//difficoltà totale se prendessi l'elemento
            start++;
            continue;
        }
        if (!pruValid(e[i], argv)) {
            start++;
            continue;
        }
        sol[nSol]=i; // prendo la soluzione corrente e ricorro su nSol+1
        bestPunti=calcolo(e, sol, bestSol, tot, nSol+1, start, diff+e[i].grado,
                          bestPunti, maxDiff, argv);
        
        start++;
    }
    return bestPunti;
}


int main(int argc, char **argv) {
    
    int *bestSol=malloc(MAX_ELEMENTI*sizeof(int));
    int i,j=0;
    int n[3];
    int tot, x=0;
    char s[100];
    float p;
    Elemento *e; // vettore di tutti gli elementi
    int *sol=malloc(MAX_ELEMENTI*sizeof(int));
    FILE *fp;
    
    // controllo parametri
    if (argc!=5) {
        puts("Errore! Inserire il numero corretto di parametri!");
        exit(1);
    }
    
    // controllo file
    if ((fp=fopen(FILE_ELEMENTI, "r"))==NULL) {
            puts("Errore! Impossibile aprire il file!");
            exit(1);
    }
        
    // lettura dei dati
    fscanf(fp, "%d %d %d", &n[NA], &n[AA], &n[AI]);
    tot=n[NA]+n[AA]+n[AI];
    
    e=(Elemento*)malloc(tot*sizeof(Elemento));
        
    // per le tre categorie...
    for (i=0; i<3; i++) {
        for (j=0; j<n[i]; j++) {
            fscanf(fp, "%s %d %f", s, &(e[x+j].grado),
                                   &(e[x+j].punti));
            e[x+j].categoria=i;
            e[x+j].nome=strdup(s);
        }
        x+=j; // tengo traccia di dove sono arrivato nel vettore di elementi
    }
    
    fclose(fp);
    
    
    // calcolo con ricorsione
    
    printf("Calcolo per %s %s %s %s\n", argv[1], argv[2], argv[3], argv[4]);
    p=calcolo(e, sol, bestSol, tot, 0, 0, 0, 0, atoi(argv[4]), argv);
    for(i=80; i-->0; printf("-"));
    printf("Punteggio: %.2f\n", p);
    displaySol(e, bestSol, MAX_ELEMENTI);
    calcolaPunteggio(e, tot, bestSol, MAX_ELEMENTI, 1);
    for(i=80; i-->0; printf("-"));
    puts("\n --- Done! ---");
    
    return 0;
}