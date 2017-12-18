#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "liste.h"
#include "atl.h"
#include "es.h"

#define FILE_ATLETI "atleti.txt"
#define FILE_ESERCIZI "esercizi.txt"

#define MAX_NOME 25
#define LUNG_CODICE 5
#define non_strutturato ;;

#ifdef _WIN32
    #define F_CLEAR "cls"
#else
    #define F_CLEAR "clear"
#endif

Atleta inputCercaAtleta(Lista);
void makeDotTxt(char*, char*);
FILE *inputStampaSuFile();

int main() {

    FILE *fp, *fEs;
    Atleta tmpAtl=newAtleta();
    Lista atleti=newAtlCollection();
    tabellaEs esercizi=newEsCollection();


    char uInput[100], fileTxt[10];
    char codice[LUNG_CODICE+1], nome[MAX_NOME+1], cognome[MAX_NOME+1];
    char categoria[MAX_NOME+1], data[11];
    
    int scelta=-1;
    int x,y;

     // file ESERCIZI
    if ((fp=fopen(FILE_ESERCIZI, "r"))==NULL){
        printf("Errore! Impossibile aprire il file \"%s\"!\n", FILE_ESERCIZI);
        exit(1);
    }
    caricaEsercizi(esercizi, fp);
    fclose(fp);
    // -------------------------------------------------------------------------

    // file degli ATLETI  (riciclo fp)
    if ((fp=fopen(FILE_ATLETI, "r"))==NULL){
        printf("Errore! Impossibile aprire il file \"%s\"!\n", FILE_ATLETI);
        exit(1);
    }
    caricaAtleti(atleti, fp);
    fclose(fp);
   
    // menu'
    for(non_strutturato) {
        system(F_CLEAR);

        puts("01. Stampa contenuto anagrafica");
        puts("02. Stampa gli atleti divisi per categoria");
        puts("03. Aggiornamento monte ore settimanali");
        puts("04. Ricerca atleta per codice o cognome parziale");
        puts("05. Aggiungi un atleta");
        puts("06. Cancella un atleta");
        for (x=80; x-->0; printf("-")); puts(""); // linea orizzontale
        puts("07. Caricare / salvare esercizi di un atleta");
        puts("08. Modificare set / ripetizioni di un esercizio di un atleta");
        puts("09. Aggiungi un esercizio");
        puts("10. Cancella un esercizio");
        for (x=80; x-->0; printf("-")); puts("");
        puts("0. Esci");
        puts("");
        printf("> ");
        scanf("%d", &scelta);

        switch (scelta) {
        case 0:
            return 0;
        case 1: // stampa contetnuto anagrafica
            fp=inputStampaSuFile();
            stampaAnagrafica(atleti, fp);
            if (fp!=stdout) fclose(fp);
            break;
        case 2: // stamapa divisi per categoria
            stampaPerCategoria(atleti);
            break;
        case 3: // aggiornamento monte ore settimanli
            if ((tmpAtl=inputCercaAtleta(atleti))==NULL) break;
            printf("Monte ore attuali: %d\n", getOreAtleta(tmpAtl));
            printf("Nuovo monte ore: ");
            scanf("%d", &x);
            modificaOreAtl(tmpAtl, x);
            puts("Monte ore aggiornato correttamente!");
            break;
        case 4: // ricerca atleta
            inputCercaAtleta(atleti);
            break;
        case 5: // aggiungi atleta
            printf("Codice:    ");
            scanf("%s", codice);
            printf("Nome:      ");
            scanf("%s", nome);
            printf("Cognome:   ");
            scanf("%s", cognome);
            printf("Cateogria: ");
            scanf("%s", categoria);
            printf("Data :     ");
            scanf("%s", data);
            printf("Monte ore: ");
            scanf("%d", &x);

            aggiungiAtletaByPar(atleti, codice, nome, cognome, categoria, data, x);
            puts("Atleta aggiunto correttamente!");

            break;
        case 6: // cancellazione atleta
            if ((tmpAtl=inputCercaAtleta(atleti))==NULL) break;
            printf("Rimuovere l'atleta trovato? [s/n] ");
            scanf("%s", uInput);
            if (tolower(uInput[0])=='s') {
               cancellaAtleta(atleti, tmpAtl);
               puts("Atleta cancellato con successo!");
            }
            break;
        case 7:
            // caricare / salvare esericizi per un atleta
            if ((tmpAtl=inputCercaAtleta(atleti))==NULL) break;

            if (eserciziCaricatiAtl(tmpAtl)) {
                // se gli esercizi sono già stati caricati
                fp=inputStampaSuFile();
                stampaTuttiEs(getListaEsercizi(tmpAtl), fp);
                break;
            }
            //else: cerco di caricare il piano esercizi per l'altleta
            makeDotTxt(fileTxt, getCodiceAtleta(tmpAtl));
            if ((fEs=fopen(fileTxt, "r"))!=NULL) {
                // se ho trovato un file con il codice dell'atleta...
                caricaPianoEsercizi(getListaEsercizi(tmpAtl), esercizi, fEs);
                puts("Piano degli esercizi caricato correttamente");
                fclose(fEs);
            } else {
                printf("Non ho trovato un piano esercizi per %s\n",
                       getCodiceAtleta(tmpAtl));
            }
            break;
        case 8:
            // modificare il numero di set/ripetizioni
            if ((tmpAtl=inputCercaAtleta(atleti))==NULL) break;

            if (!eserciziCaricatiAtl(tmpAtl)){
                printf("Esercizi non caricati per \"%s\"", getCodiceAtleta(tmpAtl));
                break;
            }
            // se gli esercizi sono già stati caricati
            printf("Nome dell'esercizio per modificare set/ripetizioni: ");
            scanf("%s", uInput);
            printf("Nuovo n* set:         "); scanf("%d", &x);
            printf("Nuovo n* ripetizioni: "); scanf("%d", &y);

            if(modificaPianoEsByName(getListaEsercizi(tmpAtl), uInput, x, y)){
                puts("Modifiche effettuate con successo!");
            } else {
                puts("Errore! Esercizio non trovato.");
            }
            break;
        case 9:
            // aggiunta di un esercizio
            // ho bisogno sia dei set/ripetizioni da mettere nella lista, sia
            // dell'esercizio da far pountare quindi del nome, della
            // categoria e del tipo di esercizio
            if ((tmpAtl=inputCercaAtleta(atleti))==NULL) break;

            printf("Nome dell'esercizio da aggiungere: ");
            scanf("%s", uInput);
            printf("Nuovo n* set:         "); scanf("%d", &x);
            printf("Nuovo n* ripetizioni: "); scanf("%d", &y);
            
            if(aggiungiEs(getListaEsercizi(tmpAtl), esercizi, uInput, x, y)) {
                puts("Esercizio aggiunto con successo!");
            } else {
                printf("Impossibile trovare l'esercizio \"%s\"!\n", uInput);
            }
            break;
        case 10:
            // cancellazione di un esercizio
            if ((tmpAtl=inputCercaAtleta(atleti))==NULL) break;

            if (!eserciziCaricatiAtl(tmpAtl)){
                printf("Esercizi non caricati per \"%s\"", getCodiceAtleta(tmpAtl));
                break;
            }
            // se gli esercizi sono già stati caricati
            printf("Nome dell'esercizio da cancellare: ");
            scanf("%s", uInput);
            
            // scorro tutti gli elementi della lista con p=head della lista
            if (cancellaPianoEsByName(getListaEsercizi(tmpAtl), uInput))
                puts("Esercizio cancellato con successo!");
            else
                puts("Errore! Esercizio non trovato!");
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

Atleta inputCercaAtleta(Lista l) {
    char c[MAX_NOME+1];
    Atleta atl;
    printf("Codice o cognome parziale dell'atleta: ");
    scanf("%s", c);
    if ((atl=cercaAtleta(l, c))!=NULL) {
        stampaAtleta(atl, stdout);
        return atl;
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

void makeDotTxt(char *dst, char *src) {
    strcpy(dst, src);
    strcat(dst, ".txt");
}
