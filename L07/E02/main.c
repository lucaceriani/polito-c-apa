#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "mie.h"
#include "atleti.h"

#define NOME_FILE "atleti.txt"

#define MAX_PERCORSO_FILE 100
#define non_strutturato ;;



int esisteCategoria(char**, char*, int);

void aggiornaCategorie(char**, char*, int*);


int main() {

    FILE *fp;
    atleta_t tmpAtleta;
    link head=NULL, p;

    char c[MAX_NOME+1], f[MAX_PERCORSO_FILE+1]; // variabili per l'input
    char **categorie; // contiene tutte le categorie
    int n_categorie=0; // importante inizializzato a 0

    int scelta=-1;
    int n=0, i=0;

    // apertura e controllo file
    if ((fp=fopen(NOME_FILE, "r"))==NULL){
        printf("Errore! Impossibile aprire il file \"%s\"!\n", NOME_FILE);
        exit(1);
    }

    // controllo della presenza e della correttezza della prima riga del file
    if (fscanf(fp, "%d\n", &n)!=1) exit(2);

    // alloco n puntatori a vettore char, un'idea poteva essere quella di
    // far puntare direttamente al campo char* categoria di ogni atleta
    // che si presenta con una categoria nuova, ma visto che si prevede
    // la cancellazione di atleti, potrebbe essere che si vada a cancellare
    // quell'atleta che ha il nome della categoria puntato da categorie, meglio
    // copiare il nome della cateoria in un posto sicuro.
    categorie=(char**)malloc(n*sizeof(char*));

    i=0;
    while(i<n && fscanf(fp, "%s %s %s %s %s %d", tmpAtleta.codice,
        tmpAtleta.cognome, tmpAtleta.nome, tmpAtleta.categoria, tmpAtleta.data,
        &tmpAtleta.ore)==6) {

        // mi salvo il cognomenome
        strcatMia(tmpAtleta.cognomenome, tmpAtleta.cognome, tmpAtleta.nome);

        // controllo e aggiunta della categoria al vettore delle categorie
        aggiornaCategorie(categorie, tmpAtleta.categoria, &n_categorie);

        // aggiungi tmpAtleta (in testa alla lista per semplicità)
        head=newNode(tmpAtleta, head);
    }
    fclose(fp);

    // menu'
    for(non_strutturato) {
        cls();

        puts("1. Stampa contenuto anagrafica");
        puts("2. Stampa gli atleti divisi per categoria");
        puts("3. Aggiornamento monte ore settimanali");
        puts("4. Ricerca atlata per codice o cognome parziale");
        puts("5. Aggiungi un atleta");
        puts("6. Cancella un atleta");
        puts("0. Esci");
        puts("");
        printf("> ");
        scanf("%d", &scelta);

        switch (scelta) {
        case 0:
            return 0;
        case 1: // stampa contetnuto anagrafica
            printf("Stampa su file? [s/n] ");
            scanf("%s", c);
            if (tolower(c[0])=='s') {
                printf("Inserisci il nome del file: ");
                scanf("%s", f);
                // riciclo fp
                if ((fp=fopen(f, "w"))==NULL) {
                    printf("Errore! Impossibile aprire il file \"%s\"", f);
                    break;
                } else {
                    fprintf(fp, "%d\n", n);
                    stampaAnagrafica(head, fp);
                    fclose(fp);
                    puts("Scrittura avvenuta con successo!");
                    break;
                }
            }
            stampaAnagrafica(head, stdout);
            break;
        case 2: // stamapa divisi per categoria
            // ciclo esterno sulle categorie
            for (i=0; i<n_categorie; i++) {
                printf(" -> %s\n", categorie[i]);
                // data la categoria categorie[i] cerco quali atleti vi
                // appartengono e li stampo
                for (p=head; p!=NULL; p=p->next) {
                    if (_comp(p->val.categoria, categorie[i])==0)
                        stampaAtleta(p, stdout);
                }
            }
            break;
        case 3: // aggiornamento monte ore settimanli
            p=inputCercaAtleta(head);
            if (p==NULL) break;
            printf("Monte ore attuali: %d\n", p->val.ore);
            printf("Nuovo monte ore: ");
            scanf("%d", &(p->val.ore));
            puts("Monte ore aggiornato correttamente!");
            break;
        case 4: // ricerca atleta
            inputCercaAtleta(head);
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

            if ((head=addTailNode(head, tmpAtleta))!=NULL) {
                // aggiorno le categorie
                aggiornaCategorie(categorie, tmpAtleta.categoria, &n_categorie);
                puts("Atleta aggiunto correttamente!");
             } else {
                puts("Impossibile aggiungere l'altleta alla lista!");
            }

            break;
        case 6: // cancellazione atleta
            // se la ricerca trova qualcosa, p diventa ilpuntatore
            // a quello che la ricera ha trovato
            if ((p=inputCercaAtleta(head))==NULL) break;
            printf("Rimuovere l'atleta trovato? [s/n] ");
            scanf("%s", c);
            if (tolower(c[0])=='s') {
                // cancella atleta con codice p->val.codice
                head=delNode(head, p->val.codice);

            }
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

void aggiornaCategorie(char **categorie, char *cat, int *n) {
    // se non esiste la creao
    if (!esisteCategoria(categorie, cat, *n)) {
        // allocazione dello spazio per salvere il nome della categoria
        categorie[*n]=(char*)malloc(strlen(cat+1));
        strcpy(categorie[*n], cat);
        (*n)++;
    }
}

int esisteCategoria(char **categorie, char *c, int n) {
    // passo ogni categoria e controllo con _comp();
    int i;
    for (i=0; i<n; i++) {
        // comparazione con la funzione personalizzata per il s.o. _comp
        if (_comp(categorie[i], c)==0) return 1;
    }
    return 0;
}
