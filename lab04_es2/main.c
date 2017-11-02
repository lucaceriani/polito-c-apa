#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NOME_FILE "sequenze.txt"

#define MAXR 80
#define MAXC 60
#define MAXP 1000
#define MAX_LUNG_PAROLA 20

#define non_strutturato ;;

int leggiPagina(char [MAXR][MAXC+1], int);
int riconosciParole(char [MAXR][MAXC+1], int, char *[MAXP], int);
int confrontaParole(char*, char*); // accetta gli indici della parole nel vettore $parole
void ordinaParole(char*[MAXP], int);
int ricercaBinaria(char *[MAXP], int, char[MAX_LUNG_PAROLA]);
struct pos convertiIndice(char*, char[MAXR][MAXC]);
void minuscola(char*);
void stampa(int, char*[MAXP]); // funzione utile per il debug

struct pos {
    int r,c;
};

int main() {

    char pagina[MAXR][MAXC+1];
    char *parole[MAXP];
    char cerca[MAX_LUNG_PAROLA+1];
    int nr, np, i, indice;
    struct pos p;

    nr=leggiPagina(pagina, MAXR);
    printf("Letto il file \"%s\"\n", NOME_FILE);
    printf("Trovat%s %d rig%s\n", nr==1?"a":"e", nr, nr==1?"a":"he");

    np=riconosciParole(pagina, nr, parole, MAXP);
    printf("Trovat%s %d parol%s\n", np==1?"o":"e", np, np==1?"a":"e");

    ordinaParole(parole, np);

    puts("\nInserisci la parola da cercare:");

    for(non_strutturato) {
        printf("> ");
        fgets(cerca, MAX_LUNG_PAROLA+1, stdin);
        sscanf(cerca, "%s", cerca);
        minuscola(cerca);

        //uscita non strutturata
        if (strcmp(cerca, "$fine")==0) exit(0);

        indice=ricercaBinaria(parole, np, cerca);

        // prima di fare la converisone dell'indice, per evitare errori
        // è meglio controllare che l'indice ritornato non sia -1 e in
        // tal caso, uscire
        if (indice==-1) {
            printf("Parola non trovata\n");
            continue;
        }

        printf("Parola \"%s\" trovata in (%d,%d) ", cerca, convertiIndice(parole[indice], pagina));

        // ricerca avanti e indietro delle parole uguali
        i=indice;
        while (confrontaParole(cerca, parole[++i])==0) {
            printf("(%d,%d) ", convertiIndice(parole[i], pagina));

        }
        i=indice;
        while (confrontaParole(cerca, parole[--i])==0) {
            printf("(%d,%d) ", convertiIndice(parole[i], pagina));
        }
        puts("");

    }

    return 0;
}

int leggiPagina(char pagina[MAXR][MAXC+1], int max_r) {

    FILE *fp;
    int i=0;

    if ((fp=fopen(NOME_FILE, "r"))==NULL) exit(1);
    //lettura riga per riga
    while (fgets(pagina[i++], MAXC+1, fp)!=NULL && i<max_r);
    fclose(fp);
    return i-1;
}

int riconosciParole(char pagina[MAXR][MAXC+1], int nr, char *parole[MAXP], int max_p) {

    int in_parola=0;
    int n_parole=0;
    int i,j, p_index=0;

    for (i=0; i<nr; i++) {
        for (j=0; pagina[i][j] && n_parole<max_p; j++) {
            if (isalpha(pagina[i][j])==0 && in_parola==1) {
                 n_parole++;
                in_parola=0;

            } else if (isalpha(pagina[i][j])) {
                pagina[i][j]=tolower(pagina[i][j]); // considero solo minuscole
                if (in_parola==0) {
                    // sono dentro una parola nuova
                    // mi salvo il puntatore
                    parole[p_index]=&pagina[i][j]; // salvo il puntatore alla parola
                    p_index++; // incremento l'indice del puntatore
                    in_parola=1; // entro nella parola
                }
            }
        }
    }

    return n_parole;
}

int confrontaParole(char *a, char *b) {

    int i=0;

    while(isalpha(a[i]) && isalpha(b[i])) {
        if (a[i]!=b[i]) {
            return (a[i]<b[i])?1:-1;
        } else {
            i++;
        }
    }

    // se le due parole sono uguali fino alla fine ma una finisce prima,
    // in ordine alfabetico viene prima quella più corta

    //se $a è la stringa più lunga...
    if (isalpha(a[i]) && isalpha(b[i])==0) {
        return -1; //n.b. $i è già aumentato di uno
    } else if (isalpha(a[i])==0 && isalpha(b[i])==0) {
        return 0;
    } else {
        return 1; // altrimenti le parole sono in ordine
    }
    //  1 = sono in ordine
    //  0 = sono uguali
    // -1 = non sono in ordine

}

void ordinaParole(char *parole[MAXP], int np) {

    // classico insertion sort

    int i,j;
    char *x;

    for (i=1; i<np; i++) {
        x=parole[i];
        j=i-1;
        while (j>=0 && confrontaParole(x,parole[j])==1) {
            parole[j+1]=parole[j];
            j--;
        }
        parole[j+1]=x;
    }
}

int ricercaBinaria(char *parole[MAXP], int np, char *cerca) {

    int l,r,m;
    l=0; r=np-1;
    while ((r-l)!=0) {
        m=(l+r)/2;
        if (confrontaParole(cerca, parole[m])==0) {
            return m;
        } else if (confrontaParole(cerca, parole[m])==1){ // proseguo nel sottovettore sx
            r=m;
        } else { // proseguo nel sottovettore dx
            l=m+1;
        }
    }
    if (confrontaParole(cerca, parole[l])==0)
        return l;
    else
        return -1;
}

struct pos convertiIndice(char *parola, char pagina[MAXR][MAXC]) {
    // attenzione! parola è già il puntatore alla parola,
    //             non è il vettore *parole[MAXP]
    struct pos p;
    p.r=(parola-pagina[0])/MAXC; //trovo la riga
    p.c=(parola-pagina[p.r]);    //trovo la colonna
    return p;
};

void stampa(int i, char *parole[MAXP]) {
    // stampa la parola dato l'indice i
    int j=0;
    while(isalpha(parole[i][j])) {
        printf("%c", parole[i][j++]);
    }
    printf(" ");
}

void minuscola(char *s) {
    for (;*s;++s) *s=tolower(*s);
}

// fine
