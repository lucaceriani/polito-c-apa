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
        if (niceOutput) printf("uscita: +2.50\n");
        punteggio+=2.5;
        // se l'ultimo elemento ha già partecipato al bonus di uscita non potrà
        // partecipare la bonus della composizione, quindi tolgo la sua
        // categoria dal contatore delle categorie
        categorie[e[sol[n-1]].categoria]--;
    } else if (niceOutput) printf("uscita: +0.00\n");
    
    // possibile bonus di composizione
    for (i=0; i<3; i++) if (categorie[i]) nCat++;
    if (nCat>1){ // se sono state presentate più di una categoria -> bonus
        if (niceOutput) printf("composizione: +2.50\n");
        punteggio+=2.5;
    } else if (niceOutput) printf("composizione: +0.00\n");
    
    free(presi); // libero il vettore
    return punteggio;
}

int pruValid(Elemento e, char **argv) {
    // controllo che l'elemento non sia troppo difficile per l'altleta
    // dato che la categoria è un enum e sono in ordine come i parametri posso
    // scrivere così
    
    if (e.grado > atoi(argv[1+e.categoria])) return 0; // se è troppo difficile
    // else
    return 1;
}

void displaySol(Elemento* e, int *sol, int n) {
    int i;
    for (i=0; i<n; i++) {
        printf("%d - %s %d %+.2f\n", e[sol[i]].categoria, e[sol[i]].nome, e[sol[i]].grado, e[sol[i]].punti);
        
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

int calcDiff(Elemento *e, int *sol, int n) {
    int i;
    int currDiff=0;
    for (i=0; i<n; i++) currDiff+=e[i].grado;
    return currDiff;
}

void greedy(Elemento *e, int tot, int *sol, int n, char **argv) {
    
    int i,j;
    int maxDiff=atoi(argv[4]);
    int currDiff=0;
    int *presi=calloc(tot, sizeof(int));
    int bestPunti;
    int leastGrade=0;
    
    // ricerca elemento con grado minimo
    for (i=0; i<tot; i++) if (e[i].grado < e[leastGrade].grado) leastGrade=i;
    // riempo tutto il vettore con elementi di grado minimo, controllo se sto
    // dentro la difficoltà massima e poi procedo a cercare quelli di punteggio
    // massimo compatibili
    for (i=0; i<n; i++) sol[i]=leastGrade;
    
    currDiff=calcDiff(e, sol, n);
    if (currDiff>maxDiff) return;
    
    for (i=0; i<n; i++) {
        // ricerca dell'elemento con il punteggio massimo
        bestPunti=-1;
        for (j=0; j<tot; j++) {
            if (pruValid(e[j], argv) && currDiff-e[i].grado+e[j].grado<=maxDiff && !presi[j]) {
                if (bestPunti==-1)
                    bestPunti=j;
                else if (e[j].punti > e[bestPunti].punti)
                    bestPunti=j;
            }
        }
        if (bestPunti==-1 && i>0) {
            bestPunti=i-1;
        }
        presi[bestPunti]=1;
        sol[i]=bestPunti;
        currDiff+=e[bestPunti].grado;
    }
    
    // voglio mettere in fondo un elemento acrobatico presente nella soluzione
    if (e[sol[n-1]].categoria!=NA) return; // ho già quello che mi serve
    
    for (i=0; i<n; i++) {
        if (e[sol[i]].categoria!=NA) {
            j=sol[n-1];
            sol[n-1]=sol[i];
            sol[i]=j;
            return;
        }
    }
    
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
    
    puts("Soluzione ottima (calcolo combinatorio)...");
    
    calcolo(e, sol, bestSol, tot, 0, 0, 0, 0, atoi(argv[4]), argv);
    for(i=80; i-->0; printf("-"));
    displaySol(e, bestSol, MAX_ELEMENTI);
    p=calcolaPunteggio(e, tot, bestSol, MAX_ELEMENTI, 1);
    printf("Punteggio: %.2f\n", p);
    
    puts("\n\nSoluzione greedy...");
    greedy(e, tot, bestSol, MAX_ELEMENTI, argv);
    for(i=80; i-->0; printf("-"));
    displaySol(e, bestSol, MAX_ELEMENTI);
    p=calcolaPunteggio(e, tot, bestSol, MAX_ELEMENTI, 1);
    printf("Punteggio: %.2f\n", p);
    
    puts("\n --- Done! ---");
    
    return 0;
}