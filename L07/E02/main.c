#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILE_ATLETI "atleti.txt"
#define FILE_ESERCIZI "esercizi.txt"
#define MAX_NOME 25
#define LUNG_CODICE 5
#define MAX_PERCORSO_FILE 100
#define non_strutturato ;;

#ifdef _WIN32
    #define F_CLEAR "cls"
    #define _comp(a,b) stricmp(a,b)
#else
    #define F_CLEAR "clear"
    #define _comp(a,b) strcasecmp(a,b)
#endif

typedef struct {
    char nome[MAX_NOME+1];
    char cognome[MAX_NOME+1];
    char cognomenome[2*MAX_NOME+1];
    char categoria[MAX_NOME+1];
    char codice[LUNG_CODICE+1];
    char data[11];
    int ore;
} Atleta;

typedef struct node {
    Atleta val;
    struct node *next;
} node_t, *link;

typedef enum {
    data, nome, codice, categoria
} campo_e;

link newNode(Atleta, link);
link addTailNode(link, Atleta);
link delNode(link, char*);

link cercaAtleta(link, char*);
link inputCercaAtleta(link);
int esisteCategoria(char**, char*, int);
int startsWith(char*, char*);
void strcatMia(char*, char*, char*);
void stampaAnagrafica(link, FILE*);
void stampaAtleta(link, FILE*);
void aggiornaCategorie(char**, char*, int*);


int main() {

    FILE *fp;
    Atleta tmpAtleta;
    link head=NULL, p;

    char c[MAX_NOME+1], f[MAX_PERCORSO_FILE+1]; // variabili per l'input
    char **categorie; // contiene tutte le categorie
    int n_categorie=0; // importante inizializzato a 0

    int scelta=-1;
    int n=0, i=0;

    /**
      *      File degli atleti
      */

    // apertura e controllo file
    if ((fp=fopen(FILE_ATLETI, "r"))==NULL){
        printf("Errore! Impossibile aprire il file \"%s\"!\n", FILE_ATLETI);
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

        //puts(tmpAtleta.codice);

        // mi salvo il cognomenome
        strcatMia(tmpAtleta.cognomenome, tmpAtleta.cognome, tmpAtleta.nome);

        // controllo e aggiunta della categoria al vettore delle categorie
        aggiornaCategorie(categorie, tmpAtleta.categoria, &n_categorie);

        // aggiungi tmpAtleta (in testa alla lista per semplicità)
        head=newNode(tmpAtleta, head);
    }
    fclose(fp);

     /**
      *      File degli esericizi
      */

    if ((fp=fopen(FILE_ESERCIZI, "r"))==NULL){
        printf("Errore! Impossibile aprire il file \"%s\"!\n", FILE_ESERCIZI);
        exit(1);
    }
    if (fscanf(fp, "%d\n", &n)!=1) exit(2);


    // menu'
    for(non_strutturato) {
        system(F_CLEAR);

        puts("1. Stampa contenuto anagrafica");
        puts("2. Stampa gli atleti divisi per categoria");
        puts("3. Aggiornamento monte ore settimanali");
        puts("4. Ricerca atleta per codice o cognome parziale");
        puts("5. Aggiungi un atleta");
        puts("6. Cancella un atleta");
        puts("----------------------------------------------------");
        puts("7. Caricare piano allenamenti");
        puts("8. ");
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
link newNode(Atleta val, link next) {
    link x = malloc(sizeof(node_t));
    if (x==NULL) return NULL;
    else {
        x->val = val;
        x->next = next;
    }
    return x;
}

link addTailNode(link h, Atleta val) {
    link x;
    if (h==NULL)
        return newNode(val, NULL);
    for (x=h; x->next!=NULL; x=x->next);
    x->next = newNode(val, NULL);
    return h;
}

link delNode(link h, char *k) {
    link x, p;
    if (h == NULL)
        return 0;
    for (x=h, p=NULL; x!=NULL; p=x, x=x->next) {
        if (_comp(x->val.codice, k)==0) {
            if (x==h)
                h = x->next;
            else
                p->next = x->next;
            free(x);
            break;
        }
    }
    return h;
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

link cercaAtleta(link h, char *s) {
    // cerca atleta è una funzione intelligente che in base alle caratteristiche
    // della stringa passata come chiave cerca o nel codice o nel cogome
    // (anche parziale).

    // prima di tutto cerco di capire se so tratta di un codice. Un codice deve
    // avere tre caratteristiche:
    //      1 - avere cinque caratteri;
    //      2 - essere nel formato %c%d
    //      3 - tolower(c) == a;
    // se non si riscontrano queste due caratteristiche si presuppone che
    // la ricerca sia fatta per cognome.
    char c;
    int d, isCodice=0;

    if (strlen(s)==LUNG_CODICE && sscanf(s, "%c%d", &c,&d) && tolower(c)=='a') {
        isCodice=1;
    }
        for (; h!=NULL; h=h->next) {
            if (isCodice) {
                if (_comp(h->val.codice, s)==0) return h;
            } else {
                if (startsWith(h->val.cognomenome, s)==1) return h;
            }
        }
    // se non sono ritornato prima vuol dire che non ho trovato niente
    return NULL;
}

void stampaAnagrafica(link h, FILE *fp) {
    for (; h!=NULL; h=h->next) stampaAtleta(h, fp);
}

void stampaAtleta(link h, FILE *fp) {
    Atleta a=h->val;
    fprintf(fp, "%s %s %s %s %s %d\n", a.codice, a.nome, a.cognome,
            a.categoria, a.data,a.ore);
}

void aggiornaCategorie(char **categorie, char *cat, int *n) {
    // se non esiste la creo
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

void strcatMia(char *dest, char *src1, char *src2) {
    strcpy(dest, "");
    strcpy(dest, src1);
    strcat(dest, src2);
}

int startsWith(char *a, char *b) {
    // case unsensitive
    int i, n=0;
    // voglio proseguire il confronto fino all'ultima lettera
    // di b (che dorevbbe essere la più corta)

    n=strlen(b);
    if (strlen(a)<n) return 0;

    for (i=0; i<n; i++)
        if (tolower(a[i])!=tolower(b[i]))
            return 0;

    return 1;
}
