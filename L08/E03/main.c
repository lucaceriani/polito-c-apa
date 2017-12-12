#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "liste.h"
#include "atl.h"
#include "es.h"

#define FILE_ATLETI "atleti.txt"
#define FILE_ESERCIZI "esercizi.txt"

#define MAX_ESERCIZIO 50
#define MAX_NOME 25
#define LUNG_CODICE 5
#define MAX_PERCORSO_FILE 100
#define non_strutturato ;;

#ifdef _WIN32
    #define F_CLEAR "cls"
#else
    #define F_CLEAR "clear"
#endif

link inputCercaAtleta(link);
int esisteCategoria(char**, char*, int);
int startsWith(const char*, const char*);
void strcatMia(char*, char*, char*);
void stampaAnagrafica(Lista*, FILE*);
void aggiornaCategorie(char**, char*, int*);

void makeDotTxt(char*, char*);

FILE *inputStampaSuFile();


int main() {

    FILE *fp, *fEs;
    Atleta tmpAtleta, *tmpAtletaP;
    Lista *atleti;
    link p;

    tabellaEs *esercizi=malloc(sizeof(tabellaEs));
    esercizio_t tmpEs;
    pianoEs_t tmpPiano, *tmpPianoP;


    char uInput[100], fileTxt[10];
    char **categorie; // contiene tutte le categorie
    int n_categorie=0; // importante inizializzato a 0

    int scelta=-1;
    int n=0, i=0, x,y;

     // file ESERCIZI
    i=0; fp=NULL;
    if ((fp=fopen(FILE_ESERCIZI, "r"))==NULL){
        printf("Errore! Impossibile aprire il file \"%s\"!\n", FILE_ESERCIZI);
        exit(1);
    }
    if (fscanf(fp, "%d\n", &n)!=1) exit(2);
    // alloco il vettore degli esercizi
    esercizi->vettEs=malloc(n*sizeof(esercizio_t));
    esercizi->nEs=n;

    while (i<n && fscanf(fp, "%s %s %s",tmpEs.nomeEs,tmpEs.catEs,tmpEs.tipoEs)==3)
        esercizi->vettEs[i++]=tmpEs;

    fclose(fp);
    // -------------------------------------------------------------------------

    // file degli ATLETI  (riciclo fp)
    atleti=initList(sizeof(Atleta));

    // apertura e controllo file
    if ((fp=fopen(FILE_ATLETI, "r"))==NULL){
        printf("Errore! Impossibile aprire il file \"%s\"!\n", FILE_ATLETI);
        exit(1);
    }

    // controllo della presenza e della correttezza della prima riga del file
    if (fscanf(fp, "%d\n", &n)!=1) exit(2);

    categorie=(char**)malloc(n*sizeof(char*));

    i=0;
    while(i<n && fscanf(fp, "%s %s %s %s %s %d\n", tmpAtleta.codice,
        tmpAtleta.cognome, tmpAtleta.nome, tmpAtleta.categoria, tmpAtleta.data,
        &tmpAtleta.ore)==6) {

        strcatMia(tmpAtleta.cognomenome, tmpAtleta.cognome, tmpAtleta.nome);
        aggiornaCategorie(categorie, tmpAtleta.categoria, &n_categorie);
        // inizializzo la lista degli esercizi

        tmpAtleta.listaEs=initList(sizeof(pianoEs_t));

        addTail(atleti, &tmpAtleta, "c");


    }
    fclose(fp);
    //getc(stdin);

    // menu'
    for(non_strutturato) {
        system(F_CLEAR);

        puts("01. Stampa contenuto anagrafica");
        puts("02. Stampa gli atleti divisi per categoria");
        puts("03. Aggiornamento monte ore settimanali");
        puts("04. Ricerca atleta per codice o cognome parziale");
        puts("05. Aggiungi un atleta");
        puts("06. Cancella un atleta");
        puts("---------------------------------------------------------------");
        puts("07. Caricare / salvare esercizi di un atleta");
        puts("08. Modificare set / ripetizioni di un esercizio di un atleta");
        puts("09. Aggiungi un esercizio");
        puts("10. Cancella un esercizio");
        puts("---------------------------------------------------------------");
        puts("0. Esci");
        puts("");
        printf("> ");
        scanf("%d", &scelta);

        switch (scelta) {
        case 0:
            return 0;
        case 1: // stampa contetnuto anagrafica
            fp=inputStampaSuFile();
            fprintf(fp, "%d\n", getNAtleti(atleti));
            stampaAnagrafica(atleti, fp);
            if (fp!=stdout) fclose(fp);
            break;
        case 2: // stamapa divisi per categoria
            // ciclo esterno sulle categorie
            for (i=0; i<n_categorie; i++) {
                printf(" -> %s\n", categorie[i]);
                // data la categoria categorie[i] cerco quali atleti vi
                // appartengono e li stampo

                for (p=getHead(atleti); p!=NULL; p=getNext(p)) {
                    tmpAtletaP=getAtletaFromNode(p);
                    if (strcasecmp(tmpAtletaP->categoria, categorie[i])==0)
                        stampaAtleta(p, stdout);
                }
            }
            break;
        case 3: // aggiornamento monte ore settimanli
            p=inputCercaAtleta(getHead(atleti));
            tmpAtletaP=getAtletaFromNode(p);
            if (p==NULL) break;
            printf("Monte ore attuali: %d\n", tmpAtletaP->ore);
            printf("Nuovo monte ore: ");
            scanf("%d", &(tmpAtletaP->ore));
            puts("Monte ore aggiornato correttamente!");
            break;
        case 4: // ricerca atleta
            inputCercaAtleta(getHead(atleti));
            break;
        case 5: // aggiungi atleta
            printf("Codice:    ");
            scanf("%s", tmpAtleta.codice);
            printf("Nome:      ");
            scanf("%s", tmpAtleta.nome);
            printf("Cognome:   ");
            scanf("%s", tmpAtleta.cognome);
            printf("Cateogria: ");
            scanf("%s", tmpAtleta.categoria);
            printf("Data :     ");
            scanf("%s", tmpAtleta.data);
            printf("Monte ore: ");
            scanf("%d", &tmpAtleta.ore);

            // salvo il cognomenome
            strcatMia(tmpAtleta.cognomenome, tmpAtleta.cognome, tmpAtleta.nome);

            addTail(atleti, &tmpAtleta, "c");
            // aggiorno le categorie
            aggiornaCategorie(categorie, tmpAtleta.categoria, &n_categorie);
            puts("Atleta aggiunto correttamente!");

            break;
        case 6: // cancellazione atleta
            // se la ricerca trova qualcosa, p diventa ilpuntatore
            // a quello che la ricera ha trovato
            if ((p=inputCercaAtleta(getHead(atleti)))==NULL) break;
            printf("Rimuovere l'atleta trovato? [s/n] ");
            scanf("%s", uInput);
            if (tolower(uInput[0])=='s') {
                // cancella atleta puntato da p
               delNode(atleti, p);
            }
            break;
        case 7:
            // caricare / salvare esericizi per un atleta
            if ((p=inputCercaAtleta(getHead(atleti)))==NULL) break;
            tmpAtletaP=getAtletaFromNode(p);

            if ((p=getHead(tmpAtletaP->listaEs))!=NULL) {
                // se gli esercizi sono già stati caricati
                fp=inputStampaSuFile();
                for (; p!=NULL; p=getNext(p))
                    stampaPianoEs(getPianoEsFromNode(p), fp);
                if (fp!=stdout) fclose(fp);
            } else {
                // cerco di caricare il piano esercizi per l'altleta
                makeDotTxt(fileTxt, tmpAtletaP->codice);
                if ((fEs=fopen(fileTxt, "r"))!=NULL) {
                    // se ho trovato un file con il codice dell'atleta...
                    while (fscanf(fEs, "%s %d %d",
                           tmpPiano.nomeEs, &tmpPiano.set, &tmpPiano.rep)==3) {
                        // TODO: modificare con la funzione aggiungiEs
                        tmpPiano.es=linkEs(esercizi, tmpPiano.nomeEs);
                        addTail(tmpAtletaP->listaEs, &tmpPiano, "c");
                    }
                    puts("Piano degli esercizi caricato correttamente");
                    fclose(fEs);
                } else {
                    printf("Non ho trovato un piano esercizi per %s\n",
                           tmpAtletaP->codice);
                }
            }
            break;
        case 8:
            // modificare il numero di set/ripetizioni
            if ((p=inputCercaAtleta(getHead(atleti)))==NULL) break;
            tmpAtletaP=getAtletaFromNode(p);

            if ((p=getHead(tmpAtletaP->listaEs))==NULL){
                printf("Esercizi non caricati per \"%s\"", tmpAtletaP->codice);
                break;
            }
            // se gli esercizi sono già stati caricati
            printf("Nome dell'esercizio per modificare set/ripetizioni: ");
            scanf("%s", uInput);
            printf("Nuovo n* set:         "); scanf("%d", &x);
            printf("Nuovo n* ripetizioni: "); scanf("%d", &y);

            // scorro tutti gli elementi della lista con p=head della lista
            for (; p!=NULL; p=getNext(p)) {
                tmpPianoP=getPianoEsFromNode(p);
                // se becco l'esercizio che vogio modificare tramite
                // la strcmp lo modifico ocn i nuovi valori appena acquisiti
                if (strcasecmp(tmpPianoP->es->nomeEs, uInput)==0) {
                    modificaPianoEs(tmpPianoP, x, y);
                    puts("Esercizio modificato con successo!");
                    break;
                }
            }
            if (p==NULL) puts("Esercizio non trovato!");
            break;
        case 9:
            // aggiunta di un esercizio
            // ho bisogno sia dei set/ripetizioni da mettere nella lista, sia
            // dell'esercizio da far pountare quindi del nome, della
            // categoria e del tipo di esercizio
            if ((p=inputCercaAtleta(getHead(atleti)))==NULL) break;
            tmpAtletaP=getAtletaFromNode(p);

            printf("Nome dell'esercizio da aggiungere: ");
            scanf("%s", uInput);
            printf("Nuovo n* set:         "); scanf("%d", &x);
            printf("Nuovo n* ripetizioni: "); scanf("%d", &y);
            
            if(aggiungiEs(tmpAtletaP->listaEs, esercizi, uInput, x, y)==0) {
                printf("Impossibile trovare l'esercizio \"%s\"!\n", uInput);
            }
            break;
        case 10:
            // cancellazione di un esercizio
            if ((p=inputCercaAtleta(getHead(atleti)))==NULL) break;
            tmpAtletaP=getAtletaFromNode(p);

            if ((p=getHead(tmpAtletaP->listaEs))==NULL){
                printf("Esercizi non caricati per \"%s\"", tmpAtletaP->codice);
                break;
            }
            // se gli esercizi sono già stati caricati
            printf("Nome dell'esercizio da cancellare: ");
            scanf("%s", uInput);
            
            // scorro tutti gli elementi della lista con p=head della lista
            for (; p!=NULL; p=getNext(p)) {
                tmpPianoP=getPianoEsFromNode(p);
                // se becco l'esercizio che vogio modificare tramite
                // la strcmp lo modifico ocn i nuovi valori appena acquisiti
                if (strcasecmp(tmpPianoP->es->nomeEs, uInput)==0) {
                    cancellaPianoEs(tmpAtletaP->listaEs, p);
                    puts("Esercizio cancellato con successo!");
                    break;
                }
            }
            if (p==NULL) puts("Esercizio non trovato!");
            break;
        default:
            puts("Comando non trovato.");
        }
        getc(stdin); // prendo il ritorno a capo della scanf
        printf("\nPremere invio per tornare al menu'... ");
        getc(stdin); // aspetto che l'utente prema invio
    }
    return 0;
}

link inputCercaAtleta(link h) {
    char c[MAX_NOME+1];
    link p;
    printf("Codice o cognome parziale dell'atleta: ");
    scanf("%s", c);
    if ((p=cercaAtleta(h, c))!=NULL) {
        stampaAtleta(p, stdout);
        return p;
    } else {
        puts("Atleta non trovato");
        return NULL;
    }
}

FILE *inputStampaSuFile() {
    FILE *fp;
    char c[3], f[100];
    printf("Stampa su file? [s/n] ");
    scanf("%s", c);
    if (tolower(c[0])=='s') {
        printf("Inserisci il nome del file: ");
        scanf("%s", f);

        if ((fp=fopen(f, "w"))==NULL) {
            printf("Errore! Impossibile aprire il file \"%s\"", f);
            printf("Stampo a video...\n");
            return stdout;
        } else {
            return fp;
        }
    } else {
        return stdout;
    }
}

void stampaAnagrafica(Lista *l, FILE *fp) {
    link h=getHead(l);
    for (; h!=NULL; h=getNext(h)) stampaAtleta(h, fp);
}

void aggiornaCategorie(char **categorie, char *cat, int *n) {
    // se non esiste la creao
    if (!esisteCategoria(categorie, cat, *n)) {
        // allocazione dello spazio per salvere il nome della categoria
        categorie[*n]=(char*)malloc(strlen(cat)+1);
        strcpy(categorie[*n], cat);
        (*n)++;
    }
}

int esisteCategoria(char **categorie, char *c, int n) {
    // passo ogni categoria e controllo con strcasecmp();
    int i;
    for (i=0; i<n; i++)
        if (strcasecmp(categorie[i], c)==0) return 1;

    return 0;
}

void strcatMia(char *dest, char *src1, char *src2) {
    strcpy(dest, "");
    strcpy(dest, src1);
    strcat(dest, src2);
}

void makeDotTxt(char *dst, char *src) {
    strcatMia(dst, src, ".txt");
}
